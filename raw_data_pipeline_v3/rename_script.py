import os

v3_dir = r"c:\Users\Kayyis\OneDrive\Documents\skripsi\koding\v3_v2"

# 1. Update text dalam file (.py, .h, .ino, .cpp)
for root, dirs, files in os.walk(v3_dir):
    for name in files:
        if name.endswith(('.py', '.h', '.cpp', '.ino')) and name != 'rename_script.py':
            filepath = os.path.join(root, name)
            try:
                with open(filepath, 'r', encoding='utf-8') as f:
                    content = f.read()
                
                # Hanya replace jika ada string 'v2'
                if 'v2' in content:
                    # hindari v3_v3_v2 dengan replace sementara
                    new_content = content.replace('v3_v2', 'TEMP_MARKER')
                    new_content = new_content.replace('v2', 'v3_v2')
                    new_content = new_content.replace('TEMP_MARKER', 'v3_v2')
                    
                    with open(filepath, 'w', encoding='utf-8') as f:
                        f.write(new_content)
            except Exception as e:
                print(f"Bisa mengabaikan file {name}: {e}")

# 2. Rename files and directories bottom-up
for root, dirs, files in os.walk(v3_dir, topdown=False):
    # Rename files
    for name in files:
        if 'v2' in name and 'v3_v2' not in name:
            old_path = os.path.join(root, name)
            new_name = name.replace('v2', 'v3_v2')
            new_path = os.path.join(root, new_name)
            os.rename(old_path, new_path)
            print(f"Renamed file: {name} -> {new_name}")
            
    # Rename directories
    for name in dirs:
        if 'v2' in name and 'v3_v2' not in name:
            old_path = os.path.join(root, name)
            new_name = name.replace('v2', 'v3_v2')
            new_path = os.path.join(root, new_name)
            os.rename(old_path, new_path)
            print(f"Renamed dir: {name} -> {new_name}")

print("Selesai mengubah nama file dan folder!")
