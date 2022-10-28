import geopy.distance

csv_file = "usa_va_charlottesville_city_residence_locations.csv"

raw_network_file = "charlottesville_raw.edges"

file1= open(raw_network_file, 'w')

with open(csv_file, 'r') as file:
    for line in file:
        arr = line.split(',')
        file1.write(arr[0] + " " + arr[1] + " " + arr[2] + "\n")

file1.close()

location = {}

with open(raw_network_file, 'r') as file:
    for line in file:
        arr = line.split(' ')
        location[int(arr[0])] = (float(arr[1]), float(arr[2]))

network_file = "charlottesville.edges"
file_2 = open(network_file, 'w')

for l1 in location.keys():
    for l2 in location.keys():
        if l1 != l2:
            dist = geopy.distance.geodesic(location[l1], location[l2]).miles
            if dist <= 0.0625: # within 100 yards
                file_2.write(str(l1) + " " + str(l2) + "\n")

file_2.close()
