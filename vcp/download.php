<?php

$fileName = isset($_REQUEST['filename'])? urldecode($_REQUEST['filename']):'';
$siteName = isset($_REQUEST['site']) ? urldecode($_REQUEST['site']):'';
$path = $_SERVER['VIDEO_DIR'];

$filePath = $path . $siteName .'/'. 'upload_videos/' . $fileName;

readfile_range($filePath);

function readfile_range($filePath,$retbytes=true) {
	$filesize = filesize($filePath);
	$offset = 0;
	$length = $filesize;
	

	if ( isset($_SERVER['HTTP_RANGE']) ) {
		// if the HTTP_RANGE header is set we're dealing with partial content
		$partialContent = true;
		
		// find the requested range
		// this might be too simplistic, apparently the client can request
		// multiple ranges, which can become pretty complex, so ignore it for now
		preg_match('/bytes=(\d+)-(\d+)?/', $_SERVER['HTTP_RANGE'], $matches);
		
		$offset = intval($matches[1]);
		
		if(isset($matches[2]) && !empty ($matches[2])){
			$length = intval($matches[2]) - $offset;
		} else {
			$length = $filesize - $offset;
		}
	} else {
		$partialContent = false;
	}

	if ( $partialContent ) {
		// output the right headers for partial content
		header('HTTP/1.1 206 Partial Content');
		header('Content-Range: bytes ' . $offset . '-' . ($offset + $length) . '/' . $filesize);
	}

	// output the regular HTTP headers
	//header('Content-Type: ' . $ctype);
	$fileName = basename($filePath);
	header('Content-Length: ' . $length);
	header('Content-Disposition: attachment; filename="' . $fileName . '"');
	header('Accept-Ranges: bytes');
	header('Connection:Keep-Alive');

	
	$fp = fopen($filePath, 'r');
	
	// seek to the requested offset, this is 0 if it's not a partial content request
	fseek($fp, $offset);
	
	$end = $offset + $length - 1;
	// Read the whole file range(from $offset to $end) by flushing small buffered chunks
	// Start buffered download, every chunk is 1Mb
	$buffer = 1*(1024*1024);
	while(!feof($fp) && ($p = ftell($fp)) <= $end) {
	
		if ($p + $buffer > $end) {
	
			// In case we're only outputtin a chunk, make sure we don't
			// read past the length
			$buffer = $end - $p + 1;
		}
		set_time_limit(0); // Reset time limit for big files
		echo fread($fp, $buffer);
		flush(); // Free up memory. Otherwise large files will trigger PHP's memory limit.
	}
	
	fclose($file);
}
?>