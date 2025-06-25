import re
import os

COMMENT_SYMBOLS = {
    '.py': '#',
    '.js': '//',
    '.ts': '//',
    '.java': '//',
    '.cpp': '//',
    '.hpp': '//',
    '.c': '//',
    '.sh': '#',
    '.rb': '#',
    '.go': '//',
}

def detect_language_and_comment(file_path):
    ext = os.path.splitext(file_path)[1]
    language = {
        '.py': 'python',
        '.js': 'javascript',
        '.ts': 'typescript',
        '.java': 'java',
        '.cpp': 'cpp',
        '.hpp': 'cpp',
        '.html': 'html',
        '.sh': 'bash',
        '.rb': 'ruby',
        '.go': 'go',
    }.get(ext, '')
    comment = COMMENT_SYMBOLS.get(ext, '#')
    return language, comment

def extract_block_by_marker(file_path, marker):
    language, comment = detect_language_and_comment(file_path)
    start_tag = f"{comment} <{marker}>"
    end_tag = f"{comment} </{marker}>"
    print(language)
    print(comment)
    with open(file_path, 'r') as f:
        lines = f.readlines()

    inside_block = False
    collected = []
    for line in lines:
        if start_tag in line:
            inside_block = True
            continue
        if end_tag in line:
            inside_block = False
            break
        if inside_block:
            collected.append(line)

    if not collected:
        raise ValueError(f"Marker <{marker}> not found in {file_path}")

    return ''.join(collected), language

def inject_code_blocks(input_path, output_path=None):
    with open(input_path, 'r') as f:
        content = f.read()

    pattern = r'<!--\s*inject:(.+?):([a-zA-Z0-9_]+)\s*-->'

    def replacer(match):
        file_path, marker = match.groups()
        try:
            code, lang = extract_block_by_marker(file_path.strip(), marker.strip())
            return f"```{lang}\n{code}```"
        except Exception as e:
            return f"**Error including `{file_path}` marker `<{marker}>`: {e}**"

    new_content = re.sub(pattern, replacer, content)

    if output_path:
        with open(output_path, 'w') as f:
            f.write(new_content)
    else:
        print(new_content)

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="Inject code snippets into a markdown file by marker.")
    parser.add_argument("markdown_file", help="Path to your source Markdown file")
    parser.add_argument("--out", help="Path to output file (optional)")
    args = parser.parse_args()
    inject_code_blocks(args.markdown_file, args.out)