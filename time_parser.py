# This program extracts the times from the myoutput.txt file and saves them to a CSV file

import csv

# Set the output list
times = {}

# Open the file containing the lines
with open("myoutput.txt") as file:
    # Read all the lines from the file
    lines = file.readlines()

    # Process the lines
    for line in lines:
        # Check if the line contains the "Time measured" string
        if "Time measured" in line:
            # Extract the time from the line
            time = line.split(" ")[-2]

            # Extract the type of time measured
            time_type = line.split(" ")[-4]

            # Add the time to the list, grouped by time type
            if time_type not in times:
                times[time_type] = [time]
            else:
                times[time_type].append(time)

# Open the CSV file in append mode
with open("times.csv", "a") as file:
    # Create a CSV writer
    writer = csv.writer(file)

    # Write the header row
    writer.writerow(times.keys())

    # Write the times
    for i in range(max([len(time_list) for time_list in times.values()])):
        row = []
        for time_type in times.keys():
            if i < len(times[time_type]):
                row.append(times[time_type][i])
            else:
                row.append("")
        writer.writerow(row)
