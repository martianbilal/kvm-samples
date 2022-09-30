#!/usr/bin/python3
import csv
import sys


helloworlddir = "/root/kvm-samples/hello-world"
replayerdir = helloworlddir + "/replayer"

ioctls = []
comma_sep_ioctls = []

def parse_strace(in_file: str):
    comma_sep_ioctl = []
    with open(in_file, "r") as syscalls:
        for syscall in syscalls:
            if "ioctl" in syscall:
                ioctls.append(syscall)
    for ioctl in ioctls:
        ioctl = ioctl.split("(")[1]
        res = ioctl.split("= ")[1].strip("\n")
        ioctl = ioctl.split(")")[0]
        ioctl = ioctl.split(",")
        ioctl[1] = ioctl[1][1:]
        ioctl[2] = ioctl[2][1:]
        print(ioctl)
        comma_sep_ioctl.append(int(ioctl[0], 16))
        comma_sep_ioctl.append(str(ioctl[1]))
        if len(ioctl) == 3:
            comma_sep_ioctl.append(int(ioctl[2], 16))
        else:
            comma_sep_ioctl.append(-1)
            
        comma_sep_ioctl.append(int(res, 16))
        comma_sep_ioctls.append(comma_sep_ioctl)
        comma_sep_ioctl = []
        

def dump_to_csv(out_file: str):
    # print(comma_sep_ioctls)
    with open(out_file, "w+") as csv_file:
        writer = csv.writer(csv_file)
        writer.writerows(comma_sep_ioctls)

def print_usage():
    print("ioctl_parse.py - helper python script for replayer module")
    print("parse an ioctl trace generated by strace and outputs csv ")
    print("separating the arguments to the ioctl.")
    print("\nUsage:\t ioctl_parse.py [input_file_address] [output_file_address]\n\n")

    return

def main():
    in_file =  "simple.log"
    out_file = "simple.csv"

    if len(sys.argv) < 3:
        print_usage();
        exit()
    
    print("[Replayer] Helper script for parsing")
    in_file = sys.argv[1]
    out_file = sys.argv[2]
    
    print("[Replayer] Parsing trace file")
    parse_strace(in_file)

    # parse_verbose_strace(in_file)
    print("[Replayer] Dumping results to CSV file")
    dump_to_csv(out_file)
    print("[Replayer] Done!!")



if __name__=='__main__':
    main()