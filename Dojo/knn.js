var fs = require('fs');

function toInt(x) {
  return parseInt(x, 10);
}

function readDataSync(filepath) {
  var rows = fs.readFileSync(filepath, {encoding: 'utf-8'}).split('\n');
  var n = rows.length;
  var data = [];
  for (var i = 1; i < n; i++) {
    var row = rows[i];
    if (row) {
      data.push(row.split(',').map(toInt));
    }
  }
  return data;
}


function getMatchCount(data, trainData) {
  var goodCount = 0;

  data.forEach(function(image) {
    var minDistance = Number.MAX_VALUE;
    var match = null;

    trainData.forEach(function(validImage) {
      var n = validImage.length;

      var distance = 0;
      for (var i = 1; i < n; i++) {
        var pixelDelta = image[i] - validImage[i];
        distance += (pixelDelta * pixelDelta);
      }

      if (distance < minDistance) {
        minDistance = distance;
        match = validImage;
      }
    });

    if (match[0] == image[0]) {
      goodCount++;
    }
  });

  return goodCount;
}

var train = readDataSync('trainingsample.csv');
var inputs = readDataSync('validationsample.csv');

var t = new Date();
var count = getMatchCount(inputs, train);
console.log(count+'/'+inputs.length+' '+(100*count/inputs.length)+'% Took: '+((new Date() - t)/1000)+' secs.');
