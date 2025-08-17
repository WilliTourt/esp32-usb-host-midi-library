import sys
import os
import shutil
import glob

def print_usage():
    print("Usage:")
    print("  python pick_example.py <example_number> [--run]")
    print("    Dry-run by default; use --run to actually replace.")
    print("    e.g. python pick_example.py 1       # dry-run")
    print("         python pick_example.py 1 --run # actually replace main.ino")
    print()
    print("  python pick_example.py --list")
    print("    Lists available examples.")
    sys.exit(1)

def list_examples():
    examples_dir = os.path.join("..", "examples")
    ino_files = sorted(glob.glob(os.path.join(examples_dir, "*.ino")))
    if not ino_files:
        print("[INFO] No .ino files found in 'examples'")
        return

    print("Available examples:")
    for filepath in ino_files:
        filename = os.path.basename(filepath)
        print(f"  {filename}")

    print("\nTo run, type: python pick_example.py <example_number> [--run]")
    print("  e.g. python pick_example.py 1       # dry-run")
    print("       python pick_example.py 1 --run # actually replace main.ino")

    sys.exit(0)


def main():
    # Show list if no argument or --list is given
    if len(sys.argv) == 1 or (len(sys.argv) == 2 and sys.argv[1] == '--list'):
        list_examples()

    # Validate arguments count
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print_usage()

    try:
        example_num = int(sys.argv[1])
        padded_num = f"{example_num:02d}"
    except ValueError:
        print("Error: Argument must be a number (e.g., 1, 01, 2, etc.)")
        print_usage()

    do_run = '--run' in sys.argv

    # Set paths
    example_pattern = f"{padded_num}_*.ino"
    examples_dir = os.path.join("..", "examples")
    src_dir = os.path.join("..", "src")
    target_file = os.path.join(src_dir, "main.ino")
    backup_file = os.path.join(src_dir, "main.ino.bak")

    # Find matching example file
    matches = glob.glob(os.path.join(examples_dir, example_pattern))
    if not matches:
        print(f"[ERROR] No example matching '{example_pattern}' found.")
        sys.exit(1)
    if len(matches) > 1:
        print(f"[ERROR] Multiple matches found for '{example_pattern}':")
        for match in matches:
            print("  -", os.path.basename(match))
        sys.exit(1)

    source_file = matches[0]

    # Dry-run display
    print("----- Dry Run -----")
    print(f"[INFO] Source : {os.path.relpath(source_file)}")
    print(f"[INFO] Target : {os.path.relpath(target_file)}")
    print(f"[INFO] Backup : {os.path.relpath(backup_file)}")
    if not do_run:
        print("\n[NOTE] This is a dry-run. To execute, add the '--run' option.")
        return

    # Perform actual replacement
    try:
        if os.path.exists(target_file):
            shutil.copyfile(target_file, backup_file)
            print(f"[INFO] Backup created: '{backup_file}'")

        shutil.copyfile(source_file, target_file)
        print(f"[SUCCESS] Replaced 'main.ino' with '{os.path.basename(source_file)}'")
    except Exception as e:
        print(f"[ERROR] {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()