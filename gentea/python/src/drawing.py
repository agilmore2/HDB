'''This is a port of teacup.c

Built against Python 2.7.4 and Pillow 2.0.0

Created on Apr 15, 2013

@author: hsk@RTi
'''

from __future__ import division
from PIL import Image, ImageDraw, ImageFont
import math


class DrawConfig(object):
    def __init__(self):
        self._fontsize = None
        self._fontface = None
        self.set_font()

        self.colors = self.get_colors()
        self.background = self.colors['white']
        self.foreground = self.colors['blue']
        self.foregroundtext = self.colors['nblue']
        self.outline = self.colors['black']

    def get_colors(self):
        '''Define all the colors used here (as RGB tuples)'''

        colors = {
                  'white' : (255, 255, 255),
                  'blue' : (100, 100, 255),
                  'nblue' : (0, 0, 140),
                  'red' : (255, 0, 0),
                  'green' : (0, 220, 0),
                  'black' : (0, 0, 0)
                 }
        return colors

    def get_fontsize(self):
        return self._fontsize

    def set_font(self, face=None, size=None):
        '''Set the font face and size. Defaul is Arial.ttf size 11.'''

        self._fontsize = size or self._fontsize or 11
        self._fontface = face or self._fontface or 'Arial.ttf'
        self._fontface = 'LiberationSans-Regular.ttf'
        self.font = ImageFont.truetype(self._fontface, self._fontsize)

DEFAULT_CONFIG = DrawConfig()

def draw_indicator_line(image,a,b,color='green',width=1,cfg=DEFAULT_CONFIG):
    '''Draw a simple (1px width) line from `a` to `b` on `image`.

    The given points should be (x, y) coordinate pairs. The `color` parameter,
    if given, should be a string corresponding to a color returned by 
    `define_colors`. If it isn't found or isn't defined, it defaults to 
    `cfg.colors['green']`.

    Returns `image`.
    '''

    fill_color = cfg.colors.get(color) or cfg.colors['green']
    draw = ImageDraw.Draw(image)
    draw.line([a, b], fill=fill_color, width=width)
    del draw
    return image

def draw_indicator_point(image, center, color='red', size=5,cfg=DEFAULT_CONFIG):
    '''Draw a simple "point" `size` pixels in diameter, centered at `center`.

    @NOTE: Only odd sizes are actually drawn. Even sizes will be identical to
    the size immediately above (so a point of "size" 8 is actually size 9).
    '''

    radius = size//2
    fill_color = cfg.colors.get(color) or cfg.colors['red']
    upper_left = tuple(c-radius for c in center)
    lower_right = tuple(c+radius for c in center)
    draw = ImageDraw.Draw(image)
    draw.ellipse([upper_left, lower_right], fill=fill_color)
    del draw
    return image

def draw_teacup(image, upper_left, size, max_fill, current_fill, label, 
                timestamp=None, cfg=DEFAULT_CONFIG):
    '''Draw a single teacup, with a few lines of data beneath.

    The upper-left point of the teacup is at `upper_left`. The `size` parameter
    may be any number (int or float) and is used to change the size of the 
    teacup that is drawn. `label` is the label for this particular teacup, and 
    `timestamp`, if given, will be appended to the data line if given (this is 
    meant to distinguish a certain data point from the timestamp at the top of 
    the image, which should be "current").

    TODO (hsk@RTi) It would be nice not to have the units hard-coded.
    '''

    x, y = upper_left
    # upperleft, upperright, lowerright, lowerleft
    cup = [(x, y), (x+30*size, y), 
           (x+20*size, y+20*size), (x+10*size, y+20*size)]

    percent_full = current_fill*100.0/max_fill

    area_full = 400*size**2 + (3200*(size**2)*percent_full)/100.0
    area_full = (math.sqrt(area_full) - 20*size)/(40*size)
    if area_full > 1.0:
        area_full = 1.0
    elif area_full < 0.0:
        area_full = 0.0

    x, y = cup[3] # Lower left
    filled_cup = cup[:]
    filled_cup[0] = (x - 10*size*area_full,
                     y - 20*size*area_full) # Upper left
    filled_cup[1] = (x + 10*size + 10*size*area_full, 
                     y - 20*size*area_full) # Upper right

    # Draw the teacup itself
    draw = ImageDraw.Draw(image)
    draw.polygon(cup, fill=cfg.background)
    draw.polygon(filled_cup, fill=cfg.foreground, outline=cfg.foreground)
    draw.line(cup+[cup[0]], fill=cfg.outline) # Add width=2 if desired

    # Clear the image where the text is going
    fontsize = cfg.get_fontsize()
    width, height = draw.textsize(label, font=cfg.font)
    label_box = [(x, y+3), (x + width, y+3 + height)]
    draw.rectangle(label_box, fill=cfg.background)

    fraction = '{:,.0f} / {:,.0f} acft.'.format(current_fill,max_fill)
    width, height = draw.textsize(fraction, font=cfg.font)
    fraction_box = [(x, y+3+fontsize), (x + width, y+3 + fontsize + height)]
    draw.rectangle(fraction_box,fill=cfg.background)

    qualifier = '' if timestamp is None else (' on {}'.format(timestamp))
    percent_string = '{:.0f}% full{}'.format(percent_full, qualifier)
    width, height = draw.textsize(percent_string, font=cfg.font)
    percent_box = [(x, y+3 + fontsize*2), (x+width, y+3 + fontsize*2 + height)]
    draw.rectangle(percent_box, fill=cfg.background)

    # Write the text
    draw.text(label_box[0], label, fill=cfg.foregroundtext, font=cfg.font)
    draw.text(fraction_box[0], fraction, fill=cfg.foregroundtext, font=cfg.font)
    draw.text(percent_box[0], percent_string, fill=cfg.foregroundtext, 
              font=cfg.font)

    # ImageDraw tends not to be GC'd
    del draw

    return image

def write_data_line(image, upper_left, label, value, units, cfg=DEFAULT_CONFIG):
    '''Write a data line: `label value units`.

    To align this correctly under a teacup with upper-left coordinates (x, y), 
    `upper_left` should be placed at:
        `(x + 10*teacup.size, y + 20*teacup.size + 42)` (assuming Arial 11 font)
    '''

    if units.lower() == 'cfs':
        text = '%s %d %s' % (label, value, units.lower())
    else:
        text = '%s %.2f %s' % (label, value, units.lower())

    write_text(image, upper_left, text, cfg=cfg)
    return image

def write_text(image, upper_left, text, color=None, bgcolor=None, draw=None, 
    cfg=DEFAULT_CONFIG):
    '''Write an arbitrary line of text.

    This can be used externally, but it's mostly used to wrap the text-writing
    for the various other functions in this module.
    '''

    if color is None:
        color = cfg.foregroundtext
    if bgcolor is None:
        bgcolor = cfg.background

    del_draw = False
    if draw is None:
        draw = ImageDraw.Draw(image)
        del_draw = True

    x, y = upper_left
    width, height = draw.textsize(text, font=cfg.font)
    draw.rectangle([upper_left, (x + width, y + height)], fill=bgcolor)
    draw.text(upper_left, text, fill=color, font=cfg.font)
    if del_draw:
        del draw
    return image


def main():
    '''Just for testing purposes. gentea.py is the main module.'''
    
    from datetime import datetime
    from random import random


    import cbt_config as defs
    # import fap_config as defs

    im = Image.open(defs.input_image).convert('RGB')
    cfg = DrawConfig()
    cfg.set_font(size=22)

    write_text(im, defs.timestamp_location, 
               datetime.now().strftime('%Y/%m/%d'), cfg=cfg)

    for att in defs.output_attributes:
        if att['attribute_type'] == 'teacup':
            draw_teacup(im, att['upper_left'], 
                        att['teacup_size'] if not callable(att['teacup_size']) 
                            else att['teacup_size'](att['capacity']), 
                        att['capacity'], att['capacity']*random(), att['label'], cfg=cfg)
        elif att['attribute_type'] == 'line':
            draw_indicator_line(im, att['point_a'], att['point_b'],
                                color=att['color'], width=att['thickness'], 
                                cfg=cfg)
        elif att['attribute_type'] == 'point':
            draw_indicator_point(im, att['center'], color=att['color'],
                                 size=att['size'], cfg=cfg)
    
    im.save(r'C:\test\teacups.png')
    im.show()


if __name__ == '__main__':
    main()
