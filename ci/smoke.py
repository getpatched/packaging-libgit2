"""Exercise repository creation through the installed libgit2 shared library."""

import ctypes
from ctypes.util import find_library
from pathlib import Path
import tempfile


library = find_library("git2")
if not library:
    raise RuntimeError("The installed libgit2 shared library was not found")
git = ctypes.CDLL(library)
git.git_repository_init.argtypes = [
    ctypes.POINTER(ctypes.c_void_p), ctypes.c_char_p, ctypes.c_uint,
]
git.git_repository_free.argtypes = [ctypes.c_void_p]
git.git_repository_free.restype = None
if git.git_libgit2_init() < 1:
    raise RuntimeError("Could not initialize libgit2")
try:
    with tempfile.TemporaryDirectory() as directory:
        repository = ctypes.c_void_p()
        if git.git_repository_init(ctypes.byref(repository), directory.encode(), 0) != 0:
            raise RuntimeError("Could not create an ordinary Git repository")
        try:
            if not (Path(directory) / ".git" / "HEAD").is_file():
                raise RuntimeError("Expected repository HEAD was not created")
        finally:
            git.git_repository_free(repository)
finally:
    git.git_libgit2_shutdown()
