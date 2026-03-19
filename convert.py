import os

def convert_to_utf8(file_path, input_enc="gb2312", output_enc="utf-8"):
    try:
        with open(file_path, "r", encoding=input_enc) as file:
            content = file.read()
        with open(file_path, "w", encoding=output_enc) as file:
            file.write(content)
        print(f"Converted {file_path} to {output_enc}")
    except Exception as e:
        print(f"Error converting {file_path}: {e}")

directory = "./View"

for root, dirs, files in os.walk(directory):
    for file in files:
        file_path = os.path.join(root, file)
        convert_to_utf8(file_path)