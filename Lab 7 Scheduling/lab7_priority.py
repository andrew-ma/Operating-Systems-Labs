import argparse
from tabulate import tabulate

class Process:
    def __init__(self, pid, burst_time, priority) -> None:
        self.pid = pid                  # Process ID
        self.burst_time = burst_time    # needed CPU time to complete
        self.priority = priority
        self.worked_time = 0           # time executed so far,  When working time == burst time, process is complete, initially start at 0
        self.t_round = 0                # turn around time, time taken for process to fully complete = wait time + burst time
        
    def __str__(self):
        return f"Process [{self.pid}]: priority {self.priority} burst {self.burst_time}, worked {self.worked_time}, turn around {self.t_round}"
        
    def copy(self):
        process_copy =  Process(self.pid, self.burst_time, self.priority)
        process_copy.worked_time = self.worked_time
        process_copy.t_round = self.t_round
        return process_copy
        
def positive_int(value):
    try:
        value = int(value)
        if value > 0:
            return value
        else:
            raise argparse.ArgumentTypeError("{} not a valid positive integer".format(value))
        
    except ValueError:
        raise argparse.ArgumentTypeError("{} not a valid positive integer".format(value))
    
        
def arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument("num_processes", type=positive_int, help="Number of processes")
    parser.add_argument("pid_to_priority_burst_time_dict", type=lambda v: {int(pid): {"priority": int(pri), "burst_time": int(burst_time)} for pid, pri, burst_time in ([pid] + v.split("_") for pid,v in (x.split(":") for x in v.split(",")))}, help="comma-separated PID:burst time, e.g. 1:1_10,2:2_20,3:3_10")
    args = parser.parse_args()
    
    if len(args.pid_to_priority_burst_time_dict) != args.num_processes:
        parser.error("`pid_to_priority_burst_time_dict` requires same number of entries as `num_processes`")

    return args
        

def choose_priority_idx(processList):
    if not processList:
        raise Exception("Empty process list")
        
    # here highest priority is one with the minimum priority number
    #  if highest priority is one with the maximum priority number, then change function to max()
    priorityNumber = min(p.priority for p in processList)
    for i in range(len(processList)):
        if priorityNumber == processList[i].priority:
            return i
    else:
        raise Exception("Can't find priority number")
    
        
def main():
    args = arguments()
    print(args)
    
    # create Process objects from arguments
    processList = [Process(pid, infodict["burst_time"], infodict["priority"]) for pid,infodict in args.pid_to_priority_burst_time_dict.items()]
    
    # Priority (non preemptive)
    
    total_time = 0
    
    order_table = []
    final_table = []
    
    i = choose_priority_idx(processList)
    
    while processList:
        p = processList[i]
        cpu_time_remaining = (p.burst_time - p.worked_time)
        
        t = cpu_time_remaining
        total_time += t
        
        p.worked_time += t
        p.t_round = total_time
        
        final_table.append(p.copy())
        
        # remove finished process from process list
        processList.pop(i)
            
        print(p)
        order_table.append((p.copy(), t))
        
        # next index is process with most priority (lowest priority number)
        try:
            i = choose_priority_idx(processList)
        except Exception:
            break
        
    ##############################################
    # Table For Order of Execution
    row1 = ["Time"]
    row2 = ["PID"]
    for p_copy, t in order_table:
        row1.append(f"{t} ms")
        row2.append(f"{p_copy.pid}")
        
    print(tabulate((row1, row2), tablefmt="pretty"))
    
    # Table For Turn Around Time and Wait Time
    total_wait_time = sum(p.t_round - p.burst_time for p in final_table)
    print(tabulate(((p.pid, p.t_round, p.t_round - p.burst_time) for p in final_table), headers=("PID", "Turn Around Time (ms)", "Wait Time (ms)"), tablefmt="pretty"))
    print(f"Average Wait Time: {total_wait_time / args.num_processes:.2f} ms")

if __name__ == "__main__":
    main()