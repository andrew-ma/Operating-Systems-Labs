import os
import pandas

# all of them start at 20 seconds
info_dict = {
    "quicksort_0": {
        "baseline": 16,  # estimate from graph number of watts
        "num": 50000000,
        "1": {
            "elapsed": 13.630,
        },
        "2": {
            "elapsed": 13.432,
        },
        "3": {
            "elapsed": 13.785,
        },
        "4": {
            "elapsed": 13.609,
        },
        "5": {
            "elapsed": 13.597,
        }
    },
    "mergesort_0": {
        "baseline": 16,
        "num": 50000000,
        "1": {
            "elapsed": 16.803,
        },
        "2": {
            "elapsed": 16.713,
        },
        "3": {
            "elapsed": 16.715,
        },
        "4": {
            "elapsed": 16.701,
        },
        "5": {
            "elapsed": 16.685,
        }
    },
    "insertionsort_0": {
        "baseline": 15,
        "num": 140000,
        "1": {
            "elapsed": 14.740,
        },
        "2": {
            "elapsed": 14.626,
        },
        "3": {
            "elapsed": 14.603,
        },
        "4": {
            "elapsed": 14.515,
        },
        "5": {
            "elapsed": 14.585,
        }
    },
    "quicksort_3": {
        "baseline": 15,
        "num": 50000000,
        "1": {
            "elapsed": 5.791,
        },
        "2": {
            "elapsed": 5.712,
        },
        "3": {
            "elapsed": 5.739,
        },
        "4": {
            "elapsed": 5.861,
        },
        "5": {
            "elapsed": 5.760,
        }
    },
    "mergesort_3": {
        "baseline": 15,
        "num": 50000000,
        "1": {
            "elapsed": 9.461,
        },
        "2": {
            "elapsed": 9.274,
        },
        "3": {
            "elapsed": 9.292,
        },
        "4": {
            "elapsed": 9.308,
        },
        "5": {
            "elapsed": 9.349,
        }
    },
    "insertionsort_3": {
        "baseline": 17,
        "num": 140000,
        "1": {
            "elapsed": 2.532,
        },
        "2": {
            "elapsed": 2.426,
        },
        "3": {
            "elapsed": 2.444,
        },
        "4": {
            "elapsed": 2.430,
        },
        "5": {
            "elapsed": 2.415,
        }
    },
}


rootdir = "logs"
for dirpath, dirnames, filenames in os.walk(rootdir):
    curfolder = os.path.basename(dirpath)
    if curfolder.startswith("insertion") or curfolder.startswith("quick") or curfolder.startswith("merge"):
        print(curfolder)
        package_power_filename = os.path.join(
            rootdir, curfolder, "[-]", "[CSV]", "Intel Core i7 7700HQ Package Power.csv")
        if os.path.exists(package_power_filename):
            data = pandas.read_csv(package_power_filename, header=None,
                    usecols=[0, 1], names=['Time', "Watts"])

            sorting_start_time = 20

            # sorting data is part of data where time is past 20 seconds
            sorting_data = data.loc[data['Time'] >= sorting_start_time]
            
            # folder name is used as key to info_dict
            algodict = info_dict[curfolder] 
            cur_start_time = sorting_start_time
            algo_rounds_avg_joules = []
            
            # looping through each algorithm information
            for i in range(1, 6):
                # looping through each round
                round_elapsed_time = algodict[str(i)]["elapsed"]
                round_data = sorting_data.loc[(sorting_data['Time'] >= cur_start_time) & (sorting_data['Time'] < (cur_start_time + round_elapsed_time))]
                cur_start_time += round_elapsed_time
                
                round_avg_watts = round_data["Watts"].mean()
                if round_data["Watts"].empty:
                    print(sorting_data['Time'])
                    input(f"no it is empty from {cur_start_time} to {(cur_start_time + round_elapsed_time)}")
                # print(f"{curfolder} [{i}]:\tavg_watts={round_avg_watts:.3f}\telapsed_seconds={round_elapsed_time}\tavg_joules={round_avg_watts * round_elapsed_time:.3f}")
                print(f"{round_avg_watts * round_elapsed_time:.3f}")
                algo_rounds_avg_joules.append(round_avg_watts * round_elapsed_time)
                
            algo_avg_joules = sum(algo_rounds_avg_joules) / len(algo_rounds_avg_joules)
            num_elements = algodict["num"]
            print(f"{curfolder} total:\tavg_joules={algo_avg_joules:.3f}\tavg_joules/element={algo_avg_joules/num_elements:.6f}\n")