import os
import zipfile
import glob
import shutil

def check_for_credentials_and_get_confirmation(filepath):
    """
    Scans a file for lines containing credential keywords, displays them,
    and asks for user confirmation to proceed.
    """
    print("--- Wi-Fi Credential Security Check ---")
    if not os.path.exists(filepath):
        print("-> wifi_logger.h not found, skipping check.")
        return True

    credential_lines = []
    keywords = ['WIFI_SSID', 'WIFI_PASSWORD']

    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            for i, line in enumerate(f, 1):
                if any(keyword in line for keyword in keywords):
                    credential_lines.append(f"  - Line {i}: {line.strip()}")
        
        if not credential_lines:
            print("-> OK: No lines containing credential keywords found.")
            return True

        print("\n[ATTENTION] The following lines related to Wi-Fi credentials were found.")
        print("Please review them carefully to ensure no personal information is included.")
        print("-" * 60)
        for line in credential_lines:
            print(line)
        print("-" * 60)
        
        while True:
            choice = input("Do you want to proceed with creating the ZIP file? (y/n): ").lower()
            if choice in ['y', 'yes']:
                return True
            if choice in ['n', 'no']:
                return False
        
    except Exception as e:
        print(f"[ERROR] Could not perform security check: {e}")
        return False


def main():
    """
    Creates a distribution ZIP file for the Arduino IDE.
    """
    try:
        root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
        
        if not check_for_credentials_and_get_confirmation(os.path.join(root_dir, 'src', 'wifi_logger.h')):
            print("\n[CANCELLED] ZIP file creation aborted by user.")
            return

        print("\n--------------------------------------------------------------")
        print("Confirmation received. Starting ZIP file creation process...")
        print("--------------------------------------------------------------")

        release_dir = os.path.join(root_dir, 'release')
        lib_props_path = os.path.join(root_dir, 'library.properties')
        keywords_path = os.path.join(root_dir, 'keywords.txt')
        src_dir = os.path.join(root_dir, 'src')
        examples_dir = os.path.join(root_dir, 'examples')
        exclude_from_src = ['main.ino', 'credentials.h', 'M5UnitSynth.*', 'M5UnitSynthDef.h', '*.bak', 'M5UnitSynthLicense.txt']

        lib_name = 'Default' 
        print(lib_name)
        if os.path.exists(lib_props_path):
            with open(lib_props_path, 'r', encoding='utf-8') as f:
                for line in f:
                    if line.strip().startswith('name='):
                        lib_name = line.strip().split('=')[1].strip()
                        break
        
        zip_filename = f"{lib_name}.zip"
        zip_filepath = os.path.join(release_dir, zip_filename)

        if not os.path.exists(release_dir):
            os.makedirs(release_dir)

        print(f"Creating ZIP file: {zip_filepath}")

        with zipfile.ZipFile(zip_filepath, 'w', zipfile.ZIP_DEFLATED) as zipf:
            print("\nProcessing root files...")
            for file_path in [lib_props_path, keywords_path]:
                if os.path.exists(file_path):
                    arcname = os.path.join(lib_name, os.path.basename(file_path))
                    zipf.write(file_path, arcname)
                    print(f"{'+ Added:':<12} {arcname}")

            print("\nProcessing 'src' directory...")
            for file_path in glob.glob(os.path.join(src_dir, '*.*')):
                basename = os.path.basename(file_path)
                if any(glob.fnmatch.fnmatch(basename, p) for p in exclude_from_src):
                    print(f"{'- Skipping:':<12} {basename}")
                    continue
                arcname = os.path.join(lib_name, 'src', basename)
                zipf.write(file_path, arcname)
                print(f"{'+ Added:':<12} {arcname}")

            print("\nProcessing 'examples' directory...")
            if os.path.isdir(examples_dir):
                files_to_add = glob.glob(os.path.join(examples_dir, '**', '*.*'), recursive=True)
                
                for file_path in files_to_add:
                    basename = os.path.basename(file_path)
                    filename_without_ext = os.path.splitext(basename)[0]
                    
                    relative_to_examples = os.path.relpath(file_path, examples_dir)
                    
                    if basename.endswith('.ino'):
                        arcname = os.path.join(lib_name, 'examples', filename_without_ext, basename)
                        print(f"{'+ Arduino:':<12} {arcname}")
                    else:
                        arcname = os.path.join(lib_name, 'examples', relative_to_examples)
                        print(f"{'+ Added:':<12} {arcname}")
                    
                    zipf.write(file_path, arcname)
            else:
                print(" ! Warning: 'examples' directory not found.")
        
        print("\n--------------------------------------------------------------")
        print(f"[SUCCESS] Arduino library ZIP created at:")
        print(f"-> {zip_filepath}")
        print("--------------------------------------------------------------")

    except Exception as e:
        print(f"\n[FATAL ERROR] An unexpected error occurred: {e}")

if __name__ == "__main__":
    main()