import re
import tkinter as tk
from tkinter import filedialog
from pathlib import Path

def extract_hero_map(file_path):
    """Extract hero IDs and names from GObjects dump file."""
    hero_map = {}
    
    # Pattern: Class Hero_[ID].[HeroName]AnimInstance
    pattern = r'Class Hero_(\d+)\.(\w+)AnimInstance'
    
    with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
        for line in f:
            match = re.search(pattern, line)
            if match:
                hero_id = int(match.group(1))
                hero_name = match.group(2)
                
                # Convert CamelCase to space-separated (optional)
                # e.g., "BlackWidow" -> "Black Widow"
                spaced_name = re.sub(r'([a-z])([A-Z])', r'\1 \2', hero_name)
                
                hero_map[hero_id] = spaced_name
    
    return hero_map

def generate_cpp_map(hero_map, output_file=None):
    """Generate C++ unordered_map code."""
    # Sort by hero ID for cleaner output
    sorted_heroes = sorted(hero_map.items())
    
    cpp_code = "#pragma once\n\n"
    cpp_code += "#include <unordered_map>\n"
    cpp_code += "#include <string>\n\n"
    cpp_code += "inline const std::unordered_map<int32_t, std::string> hero_id_map = {\n"
    
    for hero_id, hero_name in sorted_heroes:
        cpp_code += f'\t{{{hero_id}, "{hero_name}"}},\n'
    
    cpp_code += "};\n"
    
    if output_file:
        with open(output_file, 'w') as f:
            f.write(cpp_code)
        print(f"C++ map written to {output_file}")
    else:
        print(cpp_code)
    
    return cpp_code

def select_file():
    """Open file dialog to select the GObjects dump file."""
    root = tk.Tk()
    root.withdraw()  # Hide the main window
    
    file_path = filedialog.askopenfilename(
        title="Select GObjects Dump File",
        filetypes=[
            ("Text files", "*.txt"),
            ("All files", "*.*")
        ],
        initialdir="."
    )
    
    root.destroy()
    return file_path

def main():
    print("Please select the GObjects dump file...")
    input_file = select_file()
    
    if not input_file:
        print("No file selected. Exiting.")
        return
    
    print(f"Selected file: {input_file}")
    print("Extracting hero data...")
    
    hero_map = extract_hero_map(input_file)
    
    print(f"Found {len(hero_map)} heroes\n")
    
    # Ask for output file location
    root = tk.Tk()
    root.withdraw()
    
    output_file = filedialog.asksaveasfilename(
        title="Save C++ Hero Map As",
        defaultextension=".hpp",
        filetypes=[
            ("C++ Header", "*.hpp"),
            ("C++ Header", "*.h"),
            ("All files", "*.*")
        ],
        initialfile="hero_map.hpp"
    )
    
    root.destroy()
    
    if output_file:
        # Generate C++ map
        generate_cpp_map(hero_map, output_file)
        print(f"\n✓ Hero map successfully generated!")
    else:
        # Just print to console if no output file selected
        print("\nNo output file selected. Printing to console:\n")
        generate_cpp_map(hero_map)
    
    # Print a summary
    print("\nHero Summary:")
    for hero_id, hero_name in sorted(hero_map.items()):
        print(f"  {hero_id}: {hero_name}")

if __name__ == "__main__":
    main()