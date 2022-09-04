# This script takes your input json files and writes new json files as output only containing the 
# desired properties. 
# For example, it processes the igrfgridData_x json file and outputs only the latitude, longitude, and xcomponent 

import json

#list of json files to process 
files_to_process = ["igrfgridData_x.json", "igrfgridData_y.json", "igrfgridData_z.json"] 
#list of json tags to keep 
tags_to_keep = ["latitude", "longitude", "xcomponent"]

def strip_unwanted_tags(d, tags): #strips unwanted json tags, only includes desired tags 
    filtered_list = [] #the list which contains only the tags we want
    for item in d["result"]: #d["result"] gives a list of dictionaries containing our data. We iterate through this list, so item is a dictionary of our data. 
        temp_dict = {} #temporary dict which will only contain our desired tags
        for tag in tags: #for each tag we want to keep 
            if tag in item.keys(): #if we have the tag 
                temp_dict[tag] = item[tag] #add the data from this tag into our temp dict 
            else: #if we don't have the tag, show an error 
                print("Error! Item does not contain tag: " + tag) 
        filtered_list.append(temp_dict) #
    return filtered_list #return the filtered list containing only the tags we want 

def merge_tags(l1, l2): #merges tags from 2 different lists of dicts. Used to get the x, y, and z components all in one 
    pass

for file in files_to_process: #processes each file 
    f = open(file, "r")
    dict_from_file = json.loads(str(f.read())) #represents the json from the file as a python dictionary 
    f.close()

    filtered_list = strip_unwanted_tags(dict_from_file, tags_to_keep)



    break
