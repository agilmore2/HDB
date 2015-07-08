'''This is a sample configuration file for gentea.py

Each true configuration file is tailored to a specific input map.
'''

database = {
    'user' : 'myusername',
    'passwd' : 'mypassword',
    'host' : 'database_host_ip', 
    'port' : 'db_port', 
    'sid' : 'database_sid'
}

input_image = (r'C:\Projects\1191_USBR_Big T FEWS and Data\teacups'
               r'\python\work\test.png')

# This function dynamically converts capacities into relative teacup sizes.
# Edit freely (or hand-assign values to the `teacup_size` attributes).
# The function should take one float > 0 as input and return one float > 0
sizer = lambda capacity: (capacity)/25333.3

# Both of these are optional. They default to size 11 'Arial.ttf'
#font_size = 11
#font_face = 'Arial.ttf'

# Optional; defaults to (1,1). Positions the timestamp on the image.
#timestamp_location = (1, 1) # Upper-left of the "inner" image

# This is the core of the config file.
# Each output attribute is essentially a json object (python dict) containing
# the necessary information to grab a value from the database and draw it.
# Valid 'attribute_type' values are listed (and an example of each provided).
# All fields are required unless specified otherwise, but order doesn't matter.
#   'teacup'    Draws a teacup
#   'line'      Draws an indicator line
#   'point'     Draws a point
#   'value'     Writes a single text value (for things like flow)
output_attributes = [
    {
        'attribute_type' : 'teacup',    # On the map
        'upper_left' : (21, 74),        # On the map
        # 'site_id' : 100723,           # In the database
        # 'datatype_id' : 17,           # In the database
        'site_datatype_id' : 100373,    # (Optional, replaces previous two)
        'teacup_size' : 3.094,          # Precision sizing
        'capacity' : 112230.0,
        'label' : 'Unknown Site 1'
    },
    {
        'attribute_type' : 'teacup',    # On the map
        'upper_left' : (221, 274),        # On the map
        # 'site_id' : 100723,           # In the database
        # 'datatype_id' : 17,           # In the database
        'site_datatype_id' : 100373,    # (Optional, replaces previous two)
        'teacup_size' : sizer,          # Dynamic sizing
        'capacity' : 87654.0,
        'label' : 'Unknown Site 2'
    },
    {
        'attribute_type' : 'line',
        # 'color' : 'green',        # This parameter is optional
        # 'thickness' : 1,          # This parameter is optional
        'point_a' : (99, 111),
        'point_b' : (176, 161)
    },
    {
        'attribute_type' : 'point',
        # 'color' : 'red',          # This parameter is optional
        # 'size' : 5,               # This parameter is optional
        'center' : (176, 161)
    },
    {
        'attribute_type' : 'value',
        'upper_left' : (51, 167),
        'site_id' : 100004,
        'datatype_id' : 19,
        # 'site_datatype_id' : 100373
        'units' : 'Unknown Units',
        'label' : 'Unknown Value'
    }
]


if __name__ == '__main__':
    # Convenience for experimenting with sizes-- run this script by itself.
    exec(r"print '\n'.join('capacity: %s\tsize: %s' % "
         r"(a['capacity'], sizer(a['capacity'])) "
         r"for a in output_attributes if a['attribute_type'] == 'teacup')")