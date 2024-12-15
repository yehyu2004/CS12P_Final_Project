import json

# Define BPM (Beats Per Minute) and starting offset 's' in milliseconds
BPM = 142  # You can change this value based on your song's BPM
s = 4000    # Starting offset in milliseconds (set to 0 if not needed)

# Calculate the duration of one beat in milliseconds
t = 60000 / BPM / 2 / 2

# Define the key timing data as a list of lists (rows of the table)
# Replace "NULL" with None for easier handling in Python
key_timing_data = [
    ["0*t+s", "4*t+s", "8*t+s", "12*t+s"],
    ["28*t+s", "24*t+s", "20*t+s", "16*t+s"],
    ["32*t+s", "40*t+s", "36*t+s", "44*t+s"],
    ["56*t+s", "48*t+s", "60*t+s", "52*t+s"],
    ["76*t+s", "72*t+s", "68*t+s", "64*t+s"],
    ["80*t+s", "84*t+s", "88*t+s", "92*t+s"],
    ["108*t+s", "100*t+s", "104*t+s", "96*t+s"],
    ["116*t+s", "124*t+s", "112*t+s", "120*t+s"]
]

# Number of columns (adjusted comment to reflect actual number of columns)
NUM_COLUMNS = 4

# Initialize the list to hold all note objects
note_objects = []

# Function to parse a single timing expression
def parse_timing_expression(expr, t, s):
    """
    Parses a timing expression of the form 'number*t+s' and returns the time in milliseconds.
    """
    try:
        # Remove whitespace
        expr = expr.strip()
        # Ensure the expression contains both '*' and '+'
        if '*' not in expr or '+' not in expr:
            raise ValueError("Expression must contain '*' and '+'")
        
        # Split by '*' to get the number before 't'
        number_part = expr.split('*')[0]
        number = float(number_part)
        # Calculate time
        time = number * t + s
        return int(time)  # Cast to integer milliseconds
    except (IndexError, ValueError) as e:
        print(f"Error parsing timing expression '{expr}': {e}")
        return None

# Iterate over each row and column to parse the timing expressions
for row_index, row in enumerate(key_timing_data):
    for col_index, cell in enumerate(row):
        if cell is None or (isinstance(cell, str) and cell.upper() == "NULL"):
            continue  # Skip if there's no note in this cell

        # Split the cell by comma to check for hold objects
        timings = cell.split(',')
        if len(timings) == 1:
            # It's a tap note
            time = parse_timing_expression(timings[0], t, s)
            if time is not None:
                note = {
                    "time": time,            # Time in milliseconds
                    "column": col_index,     # Column index (0-based)
                    "type": "tap"            # Type of note
                }
                note_objects.append(note)
        elif len(timings) == 2:
            # It's a hold note
            start_time = parse_timing_expression(timings[0], t, s)
            end_time = parse_timing_expression(timings[1], t, s)
            if start_time is not None and end_time is not None:
                duration = end_time - start_time
                if duration > 0:
                    note = {
                        "time": start_time,    # Start time in milliseconds
                        "column": col_index,   # Column index (0-based)
                        "type": "hold",        # Type of note
                        "duration": duration   # Duration in milliseconds
                    }
                    note_objects.append(note)
                else:
                    print(f"Invalid duration for hold note in cell '{cell}' at row {row_index + 1}, column {col_index + 1}")
        else:
            print(f"Unexpected number of timings in cell '{cell}' at row {row_index + 1}, column {col_index + 1}")

# Sort the note_objects by time to ensure they are in chronological order
note_objects.sort(key=lambda x: x["time"])

# Create the final JSON structure
chart_json = {
    "note_objects": note_objects
}

# Define the output JSON file path
output_file_path = "chart4k.json"

# Write the JSON data to the file with pretty formatting
try:
    with open(output_file_path, 'w') as json_file:
        json.dump(chart_json, json_file, indent=4)
    print(f"Successfully wrote chart data to '{output_file_path}'")
except IOError as e:
    print(f"Error writing to file '{output_file_path}': {e}")
