## Owner: Md. N e H Jack ##

Github profile: https://github.com/akumathedynd

Copyright ©2021 Md. N e H Jack. All right reseved.

Disclaimer: This code is provided for educational purposes only. The author is not responsible for any damages or hazards caused by its use.

## C++ Web Crawler - Download and Extract Links

This C++ program crawls websites, downloads their HTML content, extracts links, and saves them to separate files.

### Features

* Reads URLs from a text file.
* Downloads HTML content using libcurl.
* Parses HTML using libxml2.
* Extracts links from the parsed HTML.
* Saves extracted links to separate files named after the original URL.
* Removes processed URLs from the input file.

### Requirements

* C++ compiler
* libcurl ([https://www.digitalocean.com/community/tutorials/workflow-downloading-files-curl](https://www.digitalocean.com/community/tutorials/workflow-downloading-files-curl))
* libxml2 ([https://dev.w3.org/XML/doc/xml.html](https://dev.w3.org/XML/doc/xml.html))

### Usage

1. Build the program using your preferred C++ compiler.
2. Create a text file named `urls.txt` containing one URL per line.
3. Create an output directory (e.g., `/path/to/output`).
4. Run the program, specifying the output directory:

```bash
./program_name /path/to/output
```

**Note:** Replace `program_name` with the actual executable name of your compiled program.

### Output

The program will download the HTML content of each URL in the input file. It will then extract links from the downloaded HTML and save them to separate files in the specified output directory. The filenames will be formatted as `URLs_<page_name>_<original_url>.txt`, where `<page_name>` is extracted from the URL and `<original_url>` is the original URL from the input file. The program will also remove processed URLs from the input file.

### License

**This project is licensed under the MIT License (see LICENSE file for details).**

### Contribution

We welcome contributions to this project. Feel free to submit pull requests with improvements or new features.

# Warning

## This script is intended for educational purposes only. It is designed to help you learn about scripting concepts and explore programming possibilities. It is not intended for production use or any situation where unintended consequences could have a negative impact or any legal issues.  The author is not responsible for any damages or hazards caused by its use. And, know about local laws.
