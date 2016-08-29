#! /usr/bin/env python2.7

import re
import subprocess
import os
import sys

this_repo = "legacy"
if os.path.exists("rfm12"):
    this_repo = "legacy-lib"

REV_RE = re.compile("-r \d+\s*")

exts = subprocess.check_output(["svn", "propget", "svn:externals", "-R"])

def repo_to_dest(repo):
    if repo.startswith("http://"):
        repo = repo[7:]
    elif repo.startswith("https://"):
        repo = repo[8:]
    if repo.startswith("www."):
        repo = repo[4:]
    if repo.startswith("das-labor.org/"):
        repo = repo[14:]
    repos = { "storage/LaborLib/": "legacy-lib",
              "svn/": "legacy" }
    for prefix, dest in repos.items():
        if repo.startswith(prefix):
            repo = repo[len(prefix):]
            repo = os.path.join(dest, repo)
            break

    return repo

for ext in exts.splitlines():
    sep = ext.find(" - ")
    if sep != -1:
        location = ext[:sep].strip()
        ext = ext[sep + 3:]
    ext = ext.strip()
    if ext == "":
        continue
    ext = REV_RE.sub("", ext).strip()
    try:
        repo, path = ext.split(" ")
    except Exception as e:
        print ext
    if path.startswith("http:") or path.startswith("https:"):
        repo, path = path, repo

    src = os.path.join(this_repo, location, path)
    dest = repo_to_dest(repo)

    src_parts = src.split("/")
    dest_parts = dest.split("/")

    while src_parts[0] == dest_parts[0]:
        src_parts = src_parts[1:]
        dest_parts = dest_parts[1:]

    relocator = "../" * (len(src_parts) - 1)
    dest = os.path.join(relocator, "/".join(dest_parts))
    src = os.path.join(location, path)

    print 'ln -s "%s" "%s"' % (dest, src)
