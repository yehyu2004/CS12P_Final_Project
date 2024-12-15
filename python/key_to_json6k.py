import json

# Define BPM (Beats Per Minute) and starting offset 's' in milliseconds
BPM = 142  # You can change this value based on your song's BPM
s = 4000      # Starting offset in milliseconds (set to 0 if not needed)

# Calculate the duration of one beat in milliseconds
t = 60000 / BPM

# Define the key timing data as a list of lists (rows of the table)
key_timing_data = [
    ["0*t+s, 8*t+s", "16*t+s", "20*t+s", "24*t+s", "28*t+s", "8*t+s, 16*t+s"],
    ["32*t+s, 40*t+s", "44*t+s, 40*t+s", "40*t+s, 44*t+s", "32*t+s, 40*t+s", "48*t+s, 56*t+s"],
    ["56*t+s, 62*t+s", "52*t+s, 53*t+s", "96*t+s, 104*t+s", "64*t+s, 72*t+s", "58*t+s, 82*t+s"],
    ["54*t+s, 55*t+s", "NULL", "80*t+s, 105*t+s", "72*t+s, 73*t+s", "NULL"],
    ["NULL", "104*t+s, 107*t+s", "74*t+s, 75*t+s", "NULL", "NULL"],
    ["NULL", "106*t+s, 109*t+s", "76*t+s, 77*t+s", "NULL", "NULL"],
    ["NULL", "108*t+s, 111*t+s", "78*t+s, 79*t+s", "NULL", "NULL"],
    ["NULL", "110*t+s", "NULL", "112*t+s, 114*t+s", "NULL", "NULL"],
    ["NULL", "124*t+s", "NULL", "NULL", "NULL", "NULL"]
]

# Initialize the list to hold all note objects
note_objects = []

# Iterate over each row and column to parse the timing expressions
for row_index, row in enumerate(key_timing_data):
    for col_index, cell in enumerate(row):
        # Handle special case for first (key 0) and sixth (key 5) columns containing pairs
        if col_index == 0 or col_index == 5:
            pairs = cell.split(",")
            if len(pairs) == 2:  # Ensure it contains two numbers
                try:
                    # Extract the first number
                    number_part_1 = pairs[0].split('*')[0].strip()
                    number_1 = float(number_part_1)
                    time_1 = number_1 * t + s
                    
                    # Extract the second number
                    number_part_2 = pairs[1].split('*')[0].strip()
                    number_2 = float(number_part_2)
                    time_2 = number_2 * t + s
                    
                    # Append as a pair
                    note = {
                        "times": [time_1, time_2],  # Store both times as a pair
                        "column": col_index  # Column index (0-based)
                    }
                    note_objects.append(note)
                except (IndexError, ValueError) as e:
                    print(f"Error parsing pair '{cell}' at row {row_index + 1}, column {col_index + 1}: {e}")
                    continue
            else:
                print(f"Warning: Unexpected format in '{cell}' at row {row_index + 1}, column {col_index + 1}")
        else:
            try:
                # Skip NULL values
                if cell == "NULL":
                    continue
                
                # Extract the number before '*t'
                number_part = cell.split('*')[0].strip()
                number = float(number_part)
                
                # Calculate the time using the formula time = number * t + s
                time = number * t + s
                
                # Create a note object
                note = {
                    "time": time,      # Time in milliseconds
                    "column": col_index  # Column index (0-based)
                }
                
                # Append the note to the list
                note_objects.append(note)
            except (IndexError, ValueError) as e:
                print(f"Error parsing cell '{cell}' at row {row_index + 1}, column {col_index + 1}: {e}")
                continue  # Skip this cell and continue with the next one

# Create the final JSON structure
chart_json = {
    "note_objects": note_objects
}

# Define the output JSON file path
output_file_path = "python/chart6k.json"

# Write the JSON data to the file with pretty formatting
try:
    with open(output_file_path, 'w') as json_file:
        json.dump(chart_json, json_file, indent=4)
    print(f"Successfully wrote chart data to '{output_file_path}'")
except IOError as e:
    print(f"Error writing to file '{output_file_path}': {e}")
