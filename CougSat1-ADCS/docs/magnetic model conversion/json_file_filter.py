# This script takes your input json files and writes new json files as output only containing the 
# desired properties. 
# For example, it processes the igrfgridData_x json file and outputs only the latitude, longitude, and xcomponent 

import json

#list of json files to process 
files_to_process = ["igrfgridData_x.json", "igrfgridData_y.json", "igrfgridData_z.json"] 
#list of json tags to keep 
tags_to_keep = ["latitude", "longitude", "xcomponent"]

def strip_unwanted_tags(d): #strips unwanted json tags, only includes desired tags 
    for item in d["result"]:
        print(item)
        break



for file in files_to_process: #processes each file 
    f = open(file, "r")
    dict_from_file = json.loads(str(f.read())) #represents the json from the file as a python dictionary 
    strip_unwanted_tags(dict_from_file)
    f.close()
    break
