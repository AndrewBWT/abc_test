import re
import os
import subprocess

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

def run_command_and_capture(command):
    try:
        # Split into args safely (handles quotes, spaces, etc.)
        args = shlex.split(command, posix=os.name != 'nt')
        result = subprocess.run(args, capture_output=True, text=True, check=True)
        return result.stdout
    except subprocess.CalledProcessError as e:
        return f"**Error running command `{command}`: {e.stderr or str(e)}**"
    except FileNotFoundError:
        return f"**Executable not found: `{args[0]}`**"

def inject_code_blocks(input_path, output_path=None):
    with open(input_path, 'r') as f:
        content = f.read()

    # Pattern 1: Inject from file
    file_marker_pattern = r'<!--\s*inject:(.+?):([a-zA-Z0-9_]+)\s*-->'

    # Pattern 2: Run command (optionally with language)
    run_cmd_pattern = r'<!--\s*run:(.+?)(?::([a-zA-Z0-9_]+))?\s*-->'

    def inject_replacer(match):
        file_path, marker = match.groups()
        try:
            code, lang = extract_block_by_marker(file_path.strip(), marker.strip())
            return f"```{lang}\n{code}```"
        except Exception as e:
            return f"**Error including `{file_path}` marker `<{marker}>`: {e}**"

    def run_replacer(match):
        command, lang = match.groups()
        output = run_command_and_capture(command.strip())
        if lang:
            return f"```{lang}\n{output}```"
        else:
            return output.strip()

    # Apply both patterns
    content = re.sub(file_marker_pattern, inject_replacer, content)
    content = re.sub(run_cmd_pattern, run_replacer, content)

    if output_path:
        with open(output_path, 'w') as f:
            f.write(content)
    else:
        print(content)

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="Inject code snippets and command outputs into a markdown file.")
    parser.add_argument("markdown_file", help="Path to your source Markdown file")
    parser.add_argument("--out", help="Path to output file (optional)")
    args = parser.parse_args()
    inject_code_blocks(args.markdown_file, args.out)