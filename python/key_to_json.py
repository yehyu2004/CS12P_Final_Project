import json

# Define BPM (Beats Per Minute) and starting offset 's' in milliseconds
BPM = 142  # You can change this value based on your song's BPM
s = 4000      # Starting offset in milliseconds (set to 0 if not needed)

# Calculate the duration of one beat in milliseconds
t = 60000 / BPM

# Define the key timing data as a list of lists (rows of the table)
key_timing_data = [
    ["0*t+s", "4*t+s", "6*t+s", "8*t+s"],
    ["28*t+s", "24*t+s", "20*t+s", "16*t+s"],
    ["32*t+s", "40*t+s", "36*t+s", "44*t+s"],
    ["56*t+s", "48*t+s", "60*t+s", "52*t+s"],
    ["76*t+s", "72*t+s", "68*t+s", "64*t+s"],
    ["80*t+s", "84*t+s", "88*t+s", "92*t+s"],
    ["108*t+s", "100*t+s", "104*t+s", "96*t+s"],
    ["116*t+s", "124*t+s", "112*t+s", "120*t+s"]
]

# Initialize the list to hold all note objects
note_objects = []

# Iterate over each row and column to parse the timing expressions
for row_index, row in enumerate(key_timing_data):
    for col_index, cell in enumerate(row):
        # Parse the cell string "number*t+s"
        # We'll extract the number before "*t"
        try:
            # Split the string by '*' and take the first part as the number
            number_part = cell.split('*')[0]
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
output_file_path = "chart.json"

# Write the JSON data to the file with pretty formatting
try:
    with open(output_file_path, 'w') as json_file:
        json.dump(chart_json, json_file, indent=4)
    print(f"Successfully wrote chart data to '{output_file_path}'")
except IOError as e:
    print(f"Error writing to file '{output_file_path}': {e}")
