'''Configuration file for gentea.py with basemap FAP_teacup_base'''

database = {
    'user' : 'app_user',
    'passwd' : 'ecohdb',
    'host' : 'XXX.XXX.XX.200', 
    'port' : '1521', 
    'sid' : 'ecohdb'
}

input_image = (r'../work/FAP_teacup_base.png')

# Both of these are optional. They default to size 11 'Arial.ttf'
#this doesn't actually do anything.
#font_size = 22
font_size = 40
font_face = 'Arial.ttf'

# Optional; defaults to (1,1). Positions the timestamp on the image.
timestamp_location = (281, 332) # Upper-left of the "inner" image

# This function dynamically converts capacities into relative teacup sizes.
# Edit freely (or hand-assign values to the `teacup_size` attributes).
sizer = lambda capacity: (capacity)/25333.3

# This is the core of the config file.
# Each output attribute is essentially a json object (python dict) containing
# the necessary information to grab a value from the database and draw it.
output_attributes = [
    
    #MTELFBCO
    {
        'attribute_type' : 'teacup', 
        #'upper_left' : (570, 696),
        #'upper_left' : (250, 820),
        #'upper_left' : (300, 900),
        #'upper_left' : (340, 900),
        'upper_left' : (1090, 700),
        'site_datatype_id' : 101013, 
        'teacup_size' : 1, # Mt. Elbert is too small to work on the other scale
        'capacity' : 11143,
        'label' : 'Mt. Elbert Forebay'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (783, 755),
        #'point_a' : (325, 905),
        #'point_a' : (645, 940),
        #'point_a' : (685, 940),
        'point_a' : (1095, 710),
        #'point_b' : (888, 828)
        'point_b' : (900, 825)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        #'center' : (888, 828)
        'center' : (900, 825)
    },


    # PUERESCO
    {
        'attribute_type' : 'teacup', 
        #'upper_left' : (2092, 1806),
        'upper_left' : (2295, 1806),
        'site_datatype_id' : 100450, 
        'teacup_size' : sizer,
        'capacity' : 349940,
        'label' : 'Pueblo'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (2482, 1866),
        'point_a' : (2610, 1807),
        'point_b' : (2628, 1757)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        'center' : (2628, 1757)
    },
    
    # RUERESCO
    {
        'attribute_type' : 'teacup', 
        #'upper_left' : (504, 296),
        #'upper_left' : (504, 310),
        'upper_left' : (220, 720),
        'site_datatype_id' : 100460, 
        'teacup_size' : sizer,
        'capacity' : 102373,
        'label' : 'Ruedi'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (532, 416),
        #'point_a' : (515, 330),
        'point_a' : (320, 720),
        #'point_b' : (425, 545)
        'point_b' : (405, 550)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        #'center' : (425, 545)
        'center' : (405, 550)
    },
    
    # TURQLACO
    {
        'attribute_type' : 'teacup', 
        #'upper_left' : (912, 260),
        'upper_left' : (1000, 280),
        'site_datatype_id' : 101012, 
        'teacup_size' : sizer,
        'capacity' : 129398.0,
        'label' : 'Turquoise'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (988, 440),
        #'point_a' : (1050, 380),
        'point_a' : (1010, 300),
        #'point_b' : (900, 652)
        'point_b' : (890, 645)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        #'center' : (900, 652)
        'center' : (890, 645)
    },
    
    # TWIRESCO
    {
        'attribute_type' : 'teacup', 
        #'upper_left' : (616, 1144),
        'upper_left' : (660, 1144),
        'site_datatype_id' : 101014, 
        'teacup_size' : sizer,
        'capacity' : 141000.0,
        'label' : 'Twin Lakes'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (687, 1131),
        'point_a' : (800, 1145),
        #'point_b' : (900, 855)
        'point_b' : (900, 860)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        #'center' : (900, 855)
        'center' : (900, 860)
    }
]

if __name__ == '__main__':
    # Convenience for experimenting with sizes
    exec(r"print '\n'.join('%s\t%s' % (a['capacity'], sizer(a['capacity'])) "
         r"for a in output_attributes if a['attribute_type'] == 'teacup')")
