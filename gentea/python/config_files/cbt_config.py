'''Configuration file for gentea.py with basemap CBT_teacup_base'''

database = {
    'user' : 'app_user',
    'passwd' : 'XXXXXX',
    'host' : 'XXX.XXX.XX.200', 
    'port' : '1521', 
    'sid' : 'ecohdb'
}

input_image = (r'../work/CBT_teacup_base.png')

# Both of these are optional. They default to size 11 'Arial.ttf'
#this doesn't actually do anything
#font_size = 22
font_size = 40
font_face = '/usr/share/fonts/liberation/LiberationSans-Regular.ttf'

# Optional; defaults to (1,1). Positions the timestamp on the image.
timestamp_location = (281, 332)

# This function dynamically converts capacities into relative teacup sizes.
# Edit freely (or hand-assign values to the `teacup_size` attributes).
from math import sqrt
sizer = lambda capacity: (sqrt(capacity) + 20) * 0.022

# This is the core of the config file.
# Each output attribute is essentially a json object (python dict) containing
# the necessary information to grab a value from the database and draw it.
output_attributes = [
    # CARTERCO~
    {
        'attribute_type' : 'teacup', 
        'upper_left' : (2725, 948),
        'upper_left' : (2564, 1200),
        'site_datatype_id' : 100373, 
        'teacup_size' : sizer,
        'capacity' : 112230.0,
        'label' : 'Carter'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (2737, 988),
	'point_a' : (2730, 1200),
        'point_b' : (2640, 950)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        #'center' : (2639, 945)
        'center' : (2640, 950)
    },

    # FLARESCO~
    {
        'attribute_type' : 'teacup', 
        #'upper_left' : (2800, 794),
        'upper_left' : (2675, 794),
        'site_datatype_id' : 100254, 
        'teacup_size' : sizer,
        'capacity' : 759.6,
        'label' : 'Flatiron'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (2797, 802),
        'point_a' : (2675, 802),
        'point_b' : (2607, 835)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        'center' : (2607, 835)
    },
    
    # GLARESCO~
    {
        'attribute_type' : 'teacup', 
        #'upper_left' : (1412, 848),
        'upper_left' : (1390, 820),
        'site_datatype_id' : 100596, 
        'teacup_size' : sizer,
        'capacity' : 1015,
        'label' : 'Grand Lake'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (1442, 958),
        'point_a' : (1440, 960),
        'point_b' : (1442, 1074)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        'center' : (1442, 1074)
    },
    
    # GRARESCO~
    {
        'attribute_type' : 'teacup', 
        'upper_left' : (820, 1500),
        'site_datatype_id' : 100403, 
        'teacup_size' : sizer,
        'capacity' : 539758,
        'label' : 'Granby'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        'point_a' : (1230, 1500),
        'point_b' : (1384, 1300)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        'center' : (1384, 1300)
    },
    
    # GRERESCO~
    {
        'attribute_type' : 'teacup', 
        'upper_left' : (310, 1880),
        'site_datatype_id' : 100257, 
        'teacup_size' : sizer,
        'capacity' : 153639,
        'label' : 'Green Mountain'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        'point_a' : (500, 1880),
        'point_b' : (532, 1840)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        'center' : (532, 1840)
    },
    
    # HORTOOCO~
    {
        'attribute_type' : 'teacup', 
        #'upper_left' : (2336, 252),
        'upper_left' : (2100, 252),
        'site_datatype_id' : 100408, 
        'teacup_size' : sizer,
        'capacity' : 156735,
        'label' : 'Horsetooth'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (2576, 348),
        #'point_a' : (2360, 280),
        'point_a' : (2360, 280),
        'point_b' : (2732, 433)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        'center' : (2730, 432)
    },
    
    # MARYLACO~
    {
        'attribute_type' : 'teacup', 
        #'upper_left' : (2067, 929),
        'upper_left' : (1900, 1000),
        'site_datatype_id' : 100246, 
        'teacup_size' : sizer,
        'capacity' : 927,
        'label' : "Marys"
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (2075, 924),
        'point_a' : (1925, 1000),
        'point_b' : (2020, 894)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        'center' : (2018, 892)
    },
    
    # OLYDAMCO~
    {
        'attribute_type' : 'teacup', 
        #'upper_left' : (1936, 626),
        'upper_left' : (1750, 626),
        'site_datatype_id' : 100251, 
        'teacup_size' : sizer,
        'capacity' : 2978,
        'label' : 'Lake Estes'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (2033, 720),
        'point_a' : (1835, 775),
        'point_b' : (2073, 822)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        'center' : (2073, 822)
    },

    # PINRESCO~
    {
        'attribute_type' : 'teacup', 
        #'upper_left' : (2308, 878),
        'upper_left' : (2220, 890),
        'site_datatype_id' : 100253, 
        'teacup_size' : sizer,
        'capacity' : 2181,
        'label' : 'Pinewood'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (2354, 886),
        'point_a' : (2260, 897),
        'point_b' : (2500, 850)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        'center' : (2500, 850)
    },
    
    # SHARESCO~
    {
        'attribute_type' : 'teacup', 
        #'upper_left' : (1064, 1064),
        #'upper_left' : (800, 1000),
        'upper_left' : (890, 590),
        'site_datatype_id' : 100464, 
        'teacup_size' : sizer,
        'capacity' : 18369,
        'label' : 'Shadow Mountain'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (1161, 1090),
        #'point_a' : (900, 1010),
        'point_a' : (990,  780),
        #Granby'point_a' : (1230, 1500),
        'point_b' : (1394, 1105)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        #'center' : (1380, 1132)
        'center' : (1394, 1105)
    },
    
    # WILRESCO~
    {
        'attribute_type' : 'teacup', 
        #'upper_left' : (868, 1232),
        #'upper_left' : (725, 1232),
        'upper_left' : (680, 1175),
        'site_datatype_id' : 100470, 
        'teacup_size' : sizer,
        'capacity' : 10553.0,
        'label' : 'Willow Creek'
    },
    {
        'attribute_type' : 'line',
        'color' : 'red',
        'thickness' : 2,
        #'point_a' : (972, 1250),
        #'point_a' : (800, 1250),
        'point_a' : (755, 1190),
        #'point_b' : (1148, 1276)
        'point_b' : (1135, 1265)
    },
    {
        'attribute_type' : 'point',
        'color' : 'red',
        'size' : 9,
        #'center' : (1148, 1276)
        'center' : (1135, 1265)
    }
]

if __name__ == '__main__':
    # Convenience for experimenting with sizes
    exec(r"print '\n'.join('%s\t%s' % (a['capacity'], sizer(a['capacity'])) "
         r"for a in output_attributes if a['attribute_type'] == 'teacup')")
