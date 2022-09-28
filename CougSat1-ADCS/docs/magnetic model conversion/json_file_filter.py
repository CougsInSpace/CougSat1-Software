# This script takes your input json files and writes new json files as output only containing the 
# desired properties. 
# For example, it processes the igrfgridData_x json file and outputs only the latitude, longitude, and xcomponent 

import json

#list of json files to process 
files_to_process = ["igrfgridData_x.json", "igrfgridData_y.json", "igrfgridData_z.json"] 
#list of json tags to keep 
tags_to_keep = ["latitude", "longitude", "xcomponent", "ycomponent", "zcomponent"]

def strip_unwanted_tags(d, tags): #strips unwanted json tags, only includes desired tags 
    filtered_list = [] #the list which contains only the tags we want
    for item in d["result"]: #d["result"] gives a list of dictionaries containing our data. We iterate through this list, so item is a dictionary of our data. 
        temp_dict = {} #temporary dict which will only contain our desired tags
        for tag in tags: #for each tag we want to keep 
            if tag in item.keys(): #if we have the tag 
                temp_dict[tag] = item[tag] #add the data from this tag into our temp dict 
        filtered_list.append(temp_dict) #
    return filtered_list #return the filtered list containing only the tags we want 

def merge_tags(list_of_filtered_lists): #merges all dicts from the list into one dict
    output_list = list_of_filtered_lists[0]
    for i in range(0, len(output_list)):
        for j in range(1, len(list_of_filtered_lists)):
            for key, value in list_of_filtered_lists[j][i].items():
                output_list[i][key] = value #adds the value 
    # for l1 in list_of_filtered_lists: #l1 is a list of dicts 
    #     for item in l1:
    #         for key in item.keys():
    return output_list 

temp_list_of_filtered_lists = [] #will store the filtered x, y, and z components 
for file in files_to_process: #processes each file 
    print("Processing file " + file)
    f = open(file, "r")
    dict_from_file = json.loads(str(f.read())) #represents the json from the file as a python dictionary 
    f.close()

    filtered_list = strip_unwanted_tags(dict_from_file, tags_to_keep)
    temp_list_of_filtered_lists.append(filtered_list)
    print("Finished processing file " + file)

print("Merging lists")
output_list = merge_tags(temp_list_of_filtered_lists)
print("Finished merging lists")

print("Outputting to file")
output_file = open("out.json", "w")
output_file.write(json.dumps(output_list, indent=1))
output_file.close()
print("Finished outputting to file")
