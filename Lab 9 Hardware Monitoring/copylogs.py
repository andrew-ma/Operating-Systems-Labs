import os
import pandas
import json

groupnames = ("mergesort_0", "mergesort_3", "quicksort_0", "quicksort_3", "insertionsort_0", "insertionsort_3")
# Format {"merge0": [sort1Watts, sort2Watts, sort3Watts, sort4Watts, sort5Watts]}
groupname_watts_info_dict = {k: [] for k in groupnames}

# Format {"merge0": [sort1Joules, sort2Joules, sort3Joules, sort4Joules, sort5Joules]}
groupname_joules_info_dict = {k: [] for k in groupnames}



rootdir = "logs"
dest = "out"
import shutil
for dirpath, dirnames, filenames  in os.walk(rootdir):
    curfolder = os.path.basename(dirpath)
    if curfolder.startswith("insertion") or curfolder.startswith("quick") or curfolder.startswith("merge"):
        print(curfolder)
        package_power_filename = os.path.join(rootdir, curfolder, "[-]", "[CSV]", "Intel Core i7 7700HQ Package Power.csv")
        os.makedirs(os.path.join(os.path.join(dest, curfolder)), exist_ok=True)
        shutil.copyfile(package_power_filename, os.path.join(dest, curfolder, curfolder + ".csv"))
#         if os.path.exists(package_power_filename):
#             data = pandas.read_csv(package_power_filename, header=None, usecols=[0,1], names=['Time', "Watts"])
            
#             start_sort_time = 10
#             # print("Starting Sort at", start_sort_time, "seconds")
#             base_data = data.loc[data['Time'] < start_sort_time]
            
#             base_average_watts = base_data['Watts'].mean()
#             print("\tNormal Watts", base_average_watts)
            
#             sort_data = data.loc[data['Time'] >= start_sort_time]
#             if sort_data.empty:
#                 input("Stuck on " + curfolder)
            
#             sort_average_watts = sort_data['Watts'].mean()
#             print("\tSort Watts", sort_average_watts)
            
                        
#             groupname = curfolder.split("_")[0]
            
#             actual_sort_time = sort_data['Time'].iloc[-1] - start_sort_time
#             num_elements = 1
#             # if groupname.startswith("merge") or groupname.startswith("quick"):
#             #     num_elements = 5000000
#             #     # actual_sort_time = 10
#             # elif groupname.startswith('insertion'):
#             #     num_elements = 100000
#                 # actual_sort_time = 480
            
#             diff_average_watts = sort_average_watts - base_average_watts
            
#             # save diff average watts
#             groupname_watts_info_dict[groupname].append(diff_average_watts / num_elements)
            
#             # save joules
#             groupname_joules_info_dict[groupname].append((diff_average_watts) * actual_sort_time  / num_elements)
            
#             # divide by number of elements sorted, to get watts/elements sorted and joules/elements sorted
            

# for groupname, sort_watts_list in groupname_watts_info_dict.items():
#     groupname_watts_info_dict[groupname] = [round(s, 2) for s in sort_watts_list]
    
# for groupname, sort_joules_list in groupname_joules_info_dict.items():
#     groupname_joules_info_dict[groupname] = [round(s, 2) for s in sort_joules_list]
    
    
# print("Watts\n", groupname_watts_info_dict)
# print("Joules\n", groupname_joules_info_dict)