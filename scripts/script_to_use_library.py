import os
import shutil
import subprocess
from pathlib import Path

import platform

def get_executable_path(build_dir: Path, exe_name: str, build_type: str, is_multi_config: bool) -> Path:
    suffix = ".exe" if platform.system() == "Windows" else ""
    if is_multi_config:
        return build_dir / build_type / f"{exe_name}{suffix}"
    else:
        return build_dir / f"{exe_name}{suffix}"

def read_cmake_var(cache_file: Path, var_name: str) -> str | None:
    """Reads a variable from a CMakeCache.txt file"""
    if not cache_file.exists():
        print(f"Cache file doesn't exist.")
        return None
    for line in cache_file.read_text().splitlines():
        if line.startswith(f"{var_name}"):
            return line.split("=", 1)[1]
    return None

def main():
    script_dir = Path(__file__).parent.resolve()
    project_root = script_dir.parent
    build_dir = project_root / "build"  # assuming original build is here
    cache_file = build_dir / "CMakeCache.txt"

    # Extract generator and compilers from the existing build cache
    generator = read_cmake_var(cache_file, "CMAKE_GENERATOR:INTERNAL")
    c_compiler = read_cmake_var(cache_file, "CMAKE_C_COMPILER:FILEPATH")
    cxx_compiler = read_cmake_var(cache_file, "CMAKE_CXX_COMPILER:FILEPATH")

    build_type = read_cmake_var(cache_file, "CMAKE_BUILD_TYPE:STRING") or "Debug"
    is_multi_config = any(g in generator for g in ["Visual Studio", "Xcode"])

    # Create test environment
    test_root = script_dir / "test_build"
    test_root.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(project_root / "scripts" / "script_to_use_library"/ "main_to_cpy.cpp", test_root / "main.cpp")
    shutil.copyfile(project_root / "scripts" / "script_to_use_library"/"CMakeLists_to_cpy.txt", test_root / "CMakeLists.txt")

    # Configure
    cmake_cmd = [
        "cmake", ".", f"-G{generator}",
    ]
    if c_compiler:
        cmake_cmd.append(f"-DCMAKE_C_COMPILER={c_compiler}")
    if cxx_compiler:
        cmake_cmd.append(f"-DCMAKE_CXX_COMPILER={cxx_compiler}")
    if not is_multi_config:
        cmake_cmd.append(f"-DCMAKE_BUILD_TYPE={build_type}")

    subprocess.run(cmake_cmd, cwd=test_root, check=True)

    # Build
    build_cmd = ["cmake", "--build", "."]
    if is_multi_config:
        build_cmd += ["--config", build_type]
    subprocess.run(build_cmd, cwd=test_root, check=True)

    # Run
    exe_path = get_executable_path(test_root, "test_library", build_type, is_multi_config)

    if not exe_path.exists():
        raise FileNotFoundError(f"Expected executable not found: {exe_path}")

    subprocess.run([str(exe_path)], check=True)

if __name__ == "__main__":
    main()