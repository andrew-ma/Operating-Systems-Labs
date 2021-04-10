import argparse
from tabulate import tabulate

class Process:
    def __init__(self, pid, burst_time) -> None:
        self.pid = pid                  # Process ID
        self.burst_time = burst_time    # needed CPU time to complete
        self.worked_time = 0           # time executed so far,  When working time == burst time, process is complete, initially start at 0
        self.t_round = 0                # turn around time, time taken for process to fully complete = wait time + burst time
        
    def __str__(self):
        return f"Process [{self.pid}]: burst {self.burst_time}, worked {self.worked_time}, turn around {self.t_round}"
        
    def copy(self):
        process_copy =  Process(self.pid, self.burst_time)
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
    parser.add_argument("quantum_time", type=positive_int, help="Quantum Time Unit is the unit of CPU time each process gets before moving to next process")
    parser.add_argument("pid_to_burst_time_dict", type=lambda v: {int(k):int(v) for k,v in (x.split(":") for x in v.split(","))}, help="comma-separated PID:burst time, e.g. 1:10,2:20,3:10")
    args = parser.parse_args()
    
    if len(args.pid_to_burst_time_dict) != args.num_processes:
        parser.error("`pid_to_burst_time_dict` requires same number of entries as `num_processes`")

    return args
        
        
def main():
    args = arguments()
    print(args)
    
    # create Process objects from arguments
    processList = [Process(pid, burst_time) for pid,burst_time in args.pid_to_burst_time_dict.items()]
    
    # Round Robin
    i = 0
    
    total_time = 0
    
    order_table = []
    final_table = []
    
    while processList:
        p = processList[i]
        cpu_time_remaining = (p.burst_time - p.worked_time)
        
        if cpu_time_remaining <= args.quantum_time:
            # cpu time remaining is less than quantum time, so can finish this turn
            t = cpu_time_remaining
            total_time += t
            
            p.worked_time += t
            p.t_round = total_time
            
            final_table.append(p.copy())
            
            # remove finished process from process list
            processList.pop(i)
            if processList:
                i = i % len(processList)
        else:
            # cpu time remaning is more than quantum time, so can only work up to quantum time but not finish
            t = args.quantum_time
            total_time += t
            
            p.worked_time += t
            p.t_round = total_time
            
            # move to next process
            i = (i+1) % len(processList)
            
        print(p)
        order_table.append((p.copy(), t))
        
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