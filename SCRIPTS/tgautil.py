'''
Copyright 2020 Electronic Arts Inc.

This program is is free software: you can redistribute it and/or modify it under the terms of 
the GNU General Public License as published by the Free Software Foundation, 
either version 3 of the License, or (at your option) any later version.

This program is is distributed in the hope that it will be useful, but with permitted additional restrictions 
under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
distributed with this program. You should have received a copy of the 
GNU General Public License along with permitted additional restrictions 
with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection
'''
import argparse
import io
import json
from PIL import Image
import os
import StringIO
import sys
import zipfile

def overwrite_prompt(question, default=False):
    prompt = " [Y/n] " if default else " [y/N] "
    while True:
        sys.stdout.write(question + prompt)
        choice = raw_input().lower()
        if choice == '':
            return default
        elif choice == 'y':
            return True
        elif choice == 'n':
            return False
        else:
            sys.stdout.write("\n")

def crop(tga_file):
    with Image.open(tga_file) as image:
        image = image.convert('RGBA')
        alpha = image.split()[-1]
        left, top, right, bottom = 0, 0, image.width, image.height
        found_left, found_top, found_right, found_bottom = False, False, False, False
        for y in range(0, image.height):
            if found_top and found_bottom:
                break
            for x in range(0, image.width):
                if found_top and found_bottom:
                    break
                if not found_top and alpha.getpixel((x, y)) != 0:
                    top = y
                    found_top = True
                if not found_bottom and alpha.getpixel((x, image.height - y - 1)) != 0:
                    bottom = image.height - y
                    found_bottom = True
        for x in range(0, image.width):
            if found_left and found_right:
                break
            for y in range(top, bottom):
                if found_left and found_right:
                    break
                if not found_left and alpha.getpixel((x, y)) != 0:
                    left = x
                    found_left = True
                if not found_right and alpha.getpixel((image.width - x - 1, y)) != 0:
                    right = image.width - x
                    found_right = True
        tga_data = StringIO.StringIO()
        meta = None
        if left == 0 and top == 0 and right == image.width and bottom == image.height:
            image.save(tga_data, 'TGA')
        else:
            image.crop((left, top, right, bottom)).save(tga_data, 'TGA')
            meta = json.dumps({
                'size': [image.width, image.height],
                'crop': [left, top, right, bottom]
            }, separators=(',',':'))
        return (tga_data.getvalue(), meta)

def expand(tga_data, meta, tga_file):
    with Image.open(io.BytesIO(tga_data)) as image:
        if meta:
            crop = meta['crop']
            image_size = (crop[2] - crop[0], crop[3] - crop[1])
            image = image.resize(image_size)
            expanded_crop = (crop[0], crop[1], crop[2], crop[3])
            expanded_size = (meta['size'][0], meta['size'][1])
            with Image.new('RGBA', expanded_size, (0, 0, 0, 0)) as expanded:
                expanded.paste(image, expanded_crop)
                expanded.save(tga_file)
        else:
            image.save(tga_file)

def zip(args):
    if not os.path.isdir(args.directory):
        print >> sys.stderr, '\'{}\' does not exist or is not a directory\n'.format(args.directory)
        sys.exit(1)
    tga_files = [f for f in os.listdir(args.directory) if os.path.isfile(os.path.join(args.directory, f)) and os.path.splitext(f)[1].lower() == '.tga']
    if not tga_files:
        print >> sys.stderr, '\'{}\' does not contain any TGA files\n'.format(args.directory)
        sys.exit(1)
    out_file = os.path.basename(os.path.normpath(args.directory)).upper() + '.ZIP'
    if os.path.exists(out_file):
        if not os.path.isfile(out_file):
            print >> sys.stderr, '\'{}\' already exists and is not a file\n'.format(out_file)
            sys.exit(1)
        if not args.yes and not overwrite_prompt('\'{}\' already exists, overwrite?'.format(out_file)):
            sys.exit(0)
    with zipfile.ZipFile(out_file, 'w', zipfile.ZIP_DEFLATED) as zip:
        for tga_file in tga_files:
            tga_data, meta = crop(os.path.join(args.directory, tga_file))
            zip.writestr(tga_file, tga_data)
            if meta:
                zip.writestr(os.path.splitext(tga_file)[0] + '.meta', meta)
    print 'Wrote ZIP archive \'{}\''.format(out_file)

def unzip(args):
    if not os.path.isfile(args.archive):
        print >> sys.stderr, '\'{}\' does not exist or is not a file\n'.format(args.archive)
        sys.exit(1)
    out_dir = os.path.normpath(os.path.splitext(args.archive)[0])
    if os.path.exists(out_dir):
        if not os.path.isdir(out_dir):
            print >> sys.stderr, '\'{}\' already exists and is not a directory\n'.format(out_dir)
            sys.exit(1)
        if len(os.listdir(out_dir)) > 0:
            if not args.yes and not overwrite_prompt('\'{}\' is not empty, overwrite?'.format(out_dir)):
                sys.exit(0)
    else:
        os.mkdir(out_dir)
    files = {}
    with zipfile.ZipFile(args.archive, 'r', zipfile.ZIP_DEFLATED) as zip:
        for filename in zip.namelist():
            fileparts = os.path.splitext(filename)
            name, ext = fileparts[0].lower(), fileparts[1].lower()
            data = files.setdefault(name, {'tga': None, 'meta': None})
            if data['tga'] is None and ext == '.tga':
                data['tga'] = zip.read(filename)
            elif data['meta'] is None and ext == '.meta':
                data['meta'] = json.loads(zip.read(filename).decode('ascii'))
            if data['tga'] is not None and data['meta'] is not None:
                expand(data['tga'], data['meta'], os.path.join(out_dir, name) + '.tga')
                del files[name]
    for name, data in files.items():
        expand(data['tga'], None, os.path.join(out_dir, name) + '.tga')
    print 'Extracted files to \'{}\''.format(out_dir)

parser = argparse.ArgumentParser(description='TGA archive utility.')
subparsers = parser.add_subparsers()

parser_zip = subparsers.add_parser('z', help='Build a ZIP archive from a directory of TGA files.')
parser_zip.add_argument('directory', help='Directory of TGA files.')
parser_zip.add_argument('-o', '--out', nargs='?', help='Output archive path (defaults to input directory name with ZIP extension in the current path).')
parser_zip.add_argument('-y', '--yes', action='store_true', help='Confirm overwrite of existing ZIP archives.')
parser_zip.set_defaults(func=zip)

parser_unzip = subparsers.add_parser('u', help='Extract a ZIP archive of TGA files to a directory.')
parser_unzip.add_argument('archive', help='ZIP archive of TGA files.')
parser_unzip.add_argument('-o', '--out', nargs='?', help='Output directory (defaults to directory with name of the ZIP archive in the current path).')
parser_unzip.add_argument('-y', '--yes', action='store_true', help='Confirm overwrite of files in output directory.')
parser_unzip.set_defaults(func=unzip)

args = parser.parse_args()
args.func(args)
