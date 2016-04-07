Xpiks
=====

Cross-Platform Image Keywording Software

Official website: http://ribtoks.github.io/xpiks/

Translations: https://crowdin.com/project/xpiks/

Linux build status: [![Build Status](https://travis-ci.org/Ribtoks/xpiks.svg?branch=master)](https://travis-ci.org/Ribtoks/xpiks)

Coverity scan: <a href="https://scan.coverity.com/projects/xpiks-qt">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/8498/badge.svg"/>
</a>

Xpiks is a free and open source keywording and uploading tool for microstock photographers and illustrators. It was written from scratch with aim to make it more usefull, convenient and quicker than existing tools.

**Main features:**

- ability to keyword images as well as set additional XMP/IPTC metadata (title, description, author)
- uploading images to Stock Agencies' FTP hosts (saves encrypted credentials for each host)
- keywords suggestion based on Shutterstock API or the local library
- search through loaded images by any metadata (keywords, description, title)
- ability to upload vector files with images with same name (useful for illustrators)
- automatically zip vector files with previews (useful for illustrators)
- checks for potential problems before upload (insufficient resolution, missing metadata etc.)
- automatically checks spelling of description, title and keywords
- unsaved work is backed up so you can continue work later without actual writing metadata to images
- works under Linux, OS X and Windows
- ExifTool and Curl under the hood
- written in C++ with Qt 5.4

**How to build**

Please see the BUILD.md file

**How to install**

Please see the INSTALL.md file

**How to contribute**

Please see the CONTRIBUTING.md file
