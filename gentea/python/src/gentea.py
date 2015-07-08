#!/usr/bin/python
'''This is a port of gentea.c

Rather than the (extremely) outdated GD module, this now uses Pillow. The config
file has also been updated to be more intuitive.

Built against Python 2.7.4, Pillow 2.0.0, and cx_Oracle 5.1.2

Created on Apr 15, 2013

@author: hsk@RTi
'''

from __future__ import print_function
from PIL import Image
from datetime import datetime
import sys
import read_data
import drawing

def handle_attribute(image, attribute, current_date, cursor, dcfg):
    '''Handle a single attribute.

    This includes selecting data from a database using the given cursor object 
    (if applicable) and drawing the apprpriate feature to the image.
    '''
    
    # Simple cases first...
    attr_type = attribute.get('attribute_type')
    if attr_type is None:
        raise ValueError('No attribute_type specified.')
    attr_type = attr_type.lower()
    if attr_type == 'line':
        drawing.draw_indicator_line(image, attribute.get('point_a'),
                                    attribute.get('point_b'),
                                    color=attribute.get('color', 'green'),
                                    width=attribute.get('thickness', 1),
                                    cfg=dcfg)
        print('Drew line from %s to %s.' % (attribute.get('point_a'),
                                            attribute.get('point_b')))
    elif attr_type == 'point':
        drawing.draw_indicator_point(image, attribute.get('center'),
                                     size=attribute.get('size', 5),
                                     color=attribute.get('color', 'red'),
                                     cfg=dcfg)
        print('Drew point at %s.' % (attribute.get('center'),))

    # Now on to the cases that require the database connection
    else:
        if cursor is None:
            raise ValueError('No Cursor provided; cannot get data')
        site_datatype_id = attribute.get('site_datatype_id')
        if site_datatype_id is not None:
            timestamp, current_value = read_data.readone(
                                cursor, site_datatype_id=site_datatype_id)
        else:
            site_id = attribute.get('site_id')
            if site_id is None:
                raise ValueError('No site_id specified.')
            datatype_id = attribute.get('datatype_id')
            if datatype_id is None:
                raise ValueError('No datatype_id specified.')
            timestamp, current_value = read_data.readone(cursor, 
                                                         site_id, datatype_id)
        
        upper_left = attribute.get('upper_left')
        label = attribute.get('label', 'UNKNOWN')

        if attr_type == 'teacup':
            size = attribute.get('teacup_size')
            capacity = attribute.get('capacity')
            if callable(size):
                size = size(capacity)
            if timestamp == current_date:
                timestamp = None
            drawing.draw_teacup(image, upper_left, size, capacity, 
                                current_value, label, timestamp,
                                cfg=dcfg)
            print('Drew teacup at %s.' % (upper_left,))
        elif attr_type == 'value':
            units = attribute.get('units')
            drawing.write_data_line(image,upper_left,label,current_value,units,
                                    cfg=dcfg)
            print('Wrote data line at %s.' % (upper_left,))


def generate_image(cfg, dcfg):
    '''Using the given configuration (a dict), create an image.

    This is the core controller function of the whole process. It's responsible
    for getting the database connection and creating the image based on the 
    various parameters and attributes in the configuration.
    '''

    im = Image.open(cfg.get('input_image')).convert('RGB')
    nowstring = get_timestamp_string()
    drawing.write_text(im, cfg.get('timestamp_location', (1, 1)), 
                       nowstring, cfg=dcfg)
    conn = read_data.get_connection(**cfg.get('database'))
    cursor = conn.cursor()
    for attribute in cfg.get('output_attributes', {}):
        try:
            handle_attribute(im, attribute, nowstring, cursor, dcfg)
        except ValueError, e:
            print('Unable to handle attribute %s: %s' % (attribute, e))
    conn.close()
    return im


def get_configs(cfgfile):
    '''Read the `cfgfile` for configuration parameters.

    Returns the base configuration as a dictionary, as well as a draw config 
    as a DrawConfig object.
    '''

    dcfg = drawing.DrawConfig()
    cfg = {}
    execfile(cfgfile, cfg)
    dcfg.set_font(size=cfg.get('font_size'), face=cfg.get('font_face'))
    return cfg, dcfg

def get_timestamp_string():
    '''Get the current timestamp as a string.

    Note that in gentea.c, the timestamp was actually for one day previous.
    This function returns the actual current timestamp instead.
    '''

    now = datetime.now()
    return now.strftime('%Y/%m/%d')

def main(args):
    if len(args) != 3:
        print('Usage: gentea configFile outfile')
        exit(1)
    cfgfile, outfile = args[1:]
    cfg, dcfg = get_configs(cfgfile)
    image = generate_image(cfg, dcfg)
    image.save(outfile)
    print('Finished drawing teacup diagram at {}'.format(outfile))

if __name__ == '__main__':
    main(sys.argv[:])
