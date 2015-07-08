#/bin/sh
# Usage $ script radar_station [clear-radar-cache]
# Example: $ script MKX
# Example: $ script MKX clear-radar-cache

#$1 is the radar station (MKX)
#$2 is a flag to clear the cache of radar overlays. Most overlays don't 
#   change, and don't need to be re-downloaded every few minutes.

cache="/tmp/radar-cache"

# Test for the clear-cache-flag. If so, delete the entire cache and exit.
[ "$2" = "clear-radar-cache" ] && echo "Clearing cache..." && \
                                  rm -r /tmp/radar-cache && \
                                  exit 0

# Test that the radar cache exists. If not, create it.
[ -d ${cache} ] || mkdir ${cache}

# Test for each of the overlays for the N0R (Base Reflectivity) radar image.
# If the overlay is not there, download it.
[ -f ${cache}/${1}_Topo_Short.jpg ] || wget -q -P ${cache}/ http://radar.weather.gov/ridge/Overlays/Topo/Short/${1}_Topo_Short.jpg
[ -f ${cache}/${1}_County_Short.gif ] || wget -q -P ${cache}/ http://radar.weather.gov/ridge/Overlays/County/Short/${1}_County_Short.gif
[ -f ${cache}/${1}_Highways_Short.gif ] || wget -q -P ${cache}/ http://radar.weather.gov/ridge/Overlays/Highways/Short/${1}_Highways_Short.gif
[ -f ${cache}/${1}_City_Short.gif ] || wget -q -P ${cache}/ http://radar.weather.gov/ridge/Overlays/Cities/Short/${1}_City_Short.gif

# Test for the radar timestamp file. Read it. If it doesn't exist, create it.
[ -f ${cache}/radar_timestamp ] || echo "111111" > ${cache}/radar_timestamp
latest_local=$(cat ${cache}/radar_timestamp)

# Get the latest radar time from the server and compare it to the latest known.
# This avoids downloading the same image repeatedly.
radar_time_string=$(wget -S --spider http://radar.weather.gov/ridge/RadarImg/N0R/${1}_N0R_0.gif | \
                    grep "Last-Modified:" | cut -d':' -f2)
radar_time=$(date -d "$radar_time_string" +%s)
echo "Current image is ${radar_time}, cached is ${latest_local}"

# If the local timestamp is different from the server,
# Download a new image and update the timestamp file.
# Then create a new final radar-image.gif file.
if [ "${radar_time}" -ne "${latest_local}" ]; then
   echo "Downloading updated image..."
   echo "${radar_time}" > ${cache}/radar_timestamp

   # Delete the old radar, warning, and legend layers, and replace them.
   [ -f ${cache}/${1}_N0R_0.gif ] && rm ${cache}/${1}_N0R_0.gif
   wget -q -P ${cache}/ http://radar.weather.gov/ridge/RadarImg/N0R/${1}_N0R_0.gif
   [ -f ${cache}/${1}_Warnings_0.gif ] && rm ${cache}/${1}_Warnings_0.gif
   wget -q -P ${cache}/ http://radar.weather.gov/ridge/Warnings/Short/${1}_Warnings_0.gif
   [ -f ${cache}/${1}_N0R_Legend_0.gif ] && rm ${cache}/${1}_N0R_Legend_0.gif
   wget -q -P ${cache}/ http://radar.weather.gov/ridge/Legend/N0R/${1}_N0R_Legend_0.gif


   # Delete the old final radar-image. We are about to replace it.
   [ -f ${cache}/radar-image.gif ] && rm ${cache}/radar-image.gif
   echo "Creating updated overlay..."
   # Create the final radar-image using imagemagick.
   composite -compose atop ${cache}/${1}_County_Short.gif ${cache}/${1}_Topo_Short.jpg ${cache}/radar-image.gif
   composite -compose atop ${cache}/${1}_Highways_Short.gif ${cache}/radar-image.gif ${cache}/radar-image.gif
   composite -compose atop ${cache}/${1}_City_Short.gif ${cache}/radar-image.gif ${cache}/radar-image.gif
   composite -compose atop ${cache}/${1}_Warnings_0.gif ${cache}/radar-image.gif ${cache}/radar-image.gif
   composite -compose atop ${cache}/${1}_N0R_Legend_0.gif ${cache}/radar-image.gif ${cache}/radar-image.gif
   composite -compose atop ${cache}/${1}_N0R_0.gif ${cache}/radar-image.gif ${cache}/radar-image.gif

   echo "New radar image composite created at ${cache}/radar-image.gif"
fi
exit 0 
