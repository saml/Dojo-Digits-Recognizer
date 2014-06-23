<?php
function read_csv($filename) {
	$data = array_map(function ($line) {
		$row = str_getcsv($line);
		return array(intval($row[0]), array_map("intval", array_slice($row, 1)));
	}, explode(PHP_EOL, file_get_contents($filename)));
	if (count($data[count($data)-1][1]) == 0) {
		array_pop($data);
	}
	array_shift($data);
	return $data;
}

function match($validation, $training) {
	$matches = 0;
	foreach ($validation as $v_row) {
		$min_sum = 0;
		$min_label = 0;
		$v_pixels = $v_row[1];
		$v_label = $v_row[0];
		foreach ($training as $t_row) {
			$dist = 0;
			$t_label = $t_row[0];
			$t_pixels = $t_row[1];
			for ($i = 0; $i < count($v_pixels); $i++) {
				$t = $v_pixels[$i] - $t_pixels[$i];
				$dist += $t * $t;
			}
			if ($min_sum == 0 || $dist < $min_sum) {
				$min_sum = $dist;
				$min_label = $t_label;
			}
		}
		if ($min_label == $v_label) {
			$matches++;
		}
	}
	return $matches;
}

$validation = read_csv("validationsample.csv");
$training = read_csv("trainingsample.csv");

$tz = new DateTimeZone("America/New_York");
$start = new DateTime("now", $tz);
$matches = match($validation, $training);
$end = new DateTime("now", $tz);

$elapsed = $start->diff($end);
echo "Took: " . $elapsed->format("%H:%I:%S") . "\n";
echo "Matches: " . $matches;