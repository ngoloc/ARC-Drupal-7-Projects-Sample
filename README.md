ARC-Drupal-7-Projects-Sample
============================

These are three main PHP/Drupal7 projects I have worked on at ARC - Portland State University since June 15th 2012 - September 20th 2013.

<h3>The Oregon Encyclopedia Project</h3><br/>
The purpose of the Oregon Encyclopedia (OE) is to provide a free service to the public that provides access to articles based on topics pertaining to Oregon. The OE may be used as an educational reference to students and teachers, a source of information for researchers, or a place to learn for an interested individual.
![Alt text](https://raw.github.com/ngoloc/ARC-Drupal-7-Projects-Sample/master/projects-screenshots/oep-frontpage.png "OEP front page")
<br/>
<br/>
<b>Inside the code</b><br/>
<i>Topic Management: provide the editorial coordiators, administrators abilities to manage the OEP topics and assign opened topics to authors</i>
![Alt text](https://raw.github.com/ngoloc/ARC-Drupal-7-Projects-Sample/master/projects-screenshots/TopicManagement.png "Topic Management")
<i>Article Management: provide the editorial coordiators, administrators abilities to manage the OEP articles, inspect the article workflow as well as change their states manually</i>
![Alt text](https://raw.github.com/ngoloc/ARC-Drupal-7-Projects-Sample/master/projects-screenshots/ArticleManagement.png "Topic Management")
<i>Advanced Search: search all published articles based on the user criteria</i>
![Alt text](https://raw.github.com/ngoloc/ARC-Drupal-7-Projects-Sample/master/projects-screenshots/OEP-advanced-search.png "Topic Management")
<b><i>@Difficulty: The number of published articles may be extremely big (up to 10000) and each article can contain up 1000 words. Therefore, searching for keywords needs a good algorithm and strategy to handle big data</i></b>
<br/>
<b><i>@Solution: index all words from article content in the database; load all article ids to physical memory and filter them by user searching criteria; do the SQL query to match up the keywords from user input with content words indexed for a particular article</i></b>
<br/>
<b><i>@Complexity: O(n_articles * n_keywords * log(n_words_in_article))</i></b>
<h3>VCP Video Coding Project</h3><br/>
The Regional Research Institute (RRI) is currently engaged in several research projects whose data is derived from scoring video based interviews. The methodology is sound; however, the process by which the scoring is done can be improved upon. This document outlines the ARC's proposed improvements for RRI's AMP project.
![Alt text](https://raw.github.com/ngoloc/ARC-Drupal-7-Projects-Sample/master/projects-screenshots/vcp-frontpage.png "VCP front page")
<br/>
<br/>
<b>Inside the code</b><br/>
Wrote the PHP code to upload and download videos in junk files. Used pupload plugin for HTML 5 upload.
![Alt text](https://raw.github.com/ngoloc/ARC-Drupal-7-Projects-Sample/master/projects-screenshots/vcp-upload-download.png "VCP upload/download process")

<h3>EV Road Map</h3><br/>
The goal of the EV Roadmap is to make people aware of Oregon's partnership with Portland General Electric, eTec, and Nissan North America to deploy up to 4,700 zero-emission electric vehicles and 11,210 charging systems. The EV Roadmap will also provide users with tools that can help them decide to buy into the electric vehicle revolution.
![Alt text](https://raw.github.com/ngoloc/ARC-Drupal-7-Projects-Sample/master/projects-screenshots/evroadmap-frontpage.png "VCP front page")
