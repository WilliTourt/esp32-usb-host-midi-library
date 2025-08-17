import os
import shutil

def main():
    example_path = "credentials.h.example"
    target_dir = os.path.join(".")
    target_path = os.path.join(target_dir, "credentials.h")
    backup_path = target_path + ".bak"

    if not os.path.exists(example_path):
        print(f"[ERROR] Template file not found: {example_path}")
        return

    ssid = input("Enter WIFI_SSID: ").strip()
    password = input("Enter WIFI_PASSWORD: ").strip()

    with open(example_path, "r", encoding="utf-8") as f:
        content = f.read()

    content = content.replace("{{{WIFI_SSID}}}", ssid)
    content = content.replace("{{{WIFI_PASSWORD}}}", password)

    if not os.path.exists(target_dir):
        os.makedirs(target_dir)

    if os.path.exists(target_path):
        shutil.copyfile(target_path, backup_path)
        print(f"[INFO] Existing credentials.h backed up as credentials.h.bak")

    with open(target_path, "w", encoding="utf-8") as f:
        f.write(content)

    print("\n[SUCCESS] 'credentials.h' created successfully.")
    print("Please place the generated credentials.h file in the same directory as wifi_logger.h.")
    print("Now, open 'src/wifi_logger.h' and make sure the following line is uncommented to use it:")
    print('#include "credentials.h"')

if __name__ == "__main__":
    main()
