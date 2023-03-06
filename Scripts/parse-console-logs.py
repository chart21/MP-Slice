import csv
import re
import sys

# A function to extract the time measurements from a string
def extract_time_measurements(s):
    # Use a regular expression to find all the times measured
    time_measured_regex = re.compile(r'Time measured to (\w+ \w+): ([0-9.]+)s')
    time_measurements = time_measured_regex.findall(s)

    # Return the time measurements as a dictionary
    return {time_measurement[0]: time_measurement[1] for time_measurement in time_measurements}

# A function to write the time measurements from multiple files to a CSV file
def write_time_measurements_to_csv(time_measurements, csv_filename):
    # Open the CSV file for writing
    with open(csv_filename, 'w') as csv_file:
        # Create a CSV writer
        writer = csv.writer(csv_file)

        # Write the header row
        writer.writerow(['File'] + list(time_measurements[0].keys()))

        # Write the time measurements for each file
        for file_time_measurements in time_measurements:
            # Write the time measurements to the CSV file
            writer.writerow([file_time_measurements[0]] + list(file_time_measurements[1].values()))

# Read input from the terminal and extract the time measurements
time_measurements = []
current_file = ''
last_100000_lines = []
for line in sys.stdin:
    # Check if the line contains a filename
    filename_match = re.match(r'File: (\S+)', line)
    if filename_match:
        # Update the current filename
        current_file = filename_match.group(1)
        time_measurements.append({current_file: {}})
    else:
        # Keep only the last 100000 lines
        last_100000_lines.append(line)
        if len(last_100000_lines) > 100000:
            last_100000_lines.pop(0)

# Extract the time measurements from the last 100000 lines
for line in last_100000_lines:
    # Extract the time measurements from the line
    line_time_measurements = extract_time_measurements(line)
    if line_time_measurements:
        # Add the time measurements to the current file
        time_measurements[-1][current_file].update(line_time_measurements)

# Write the time measurements to a CSV file
write_time_measurements_to_csv(time_measurements, 'time_measurements.csv')

