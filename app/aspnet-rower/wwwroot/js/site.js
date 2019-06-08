"use strict";

var connection = new signalR.HubConnectionBuilder().withUrl("/feed").build();
var speedValue = document.getElementById("currentSpeed");
var countValue = document.getElementById("currentCount");
var resetButton = document.getElementById("reset");

window.onload = function () {
  var ctx = document.getElementById('chartCanvas').getContext('2d');

  window.myChart = new Chart(ctx, {
    type: 'line',
    data: {
      datasets: [{
        label: 'Speed',
        data: []
      }]
    },

    options: {
      scales: {
        xAxes: [{
          type: 'realtime',
          delay: 0,
          duration: 20000
        }],
        yAxes: [{
          ticks: {
            suggestedMin: 0,
            suggestedMax: 50
          }
        }]
      }
    }
  });

  connection.start().then(function () {
    console.log("Connected");
  }).catch(function (err) {
    return console.error(err.toString());
  });

  resetButton.addEventListener("click", function () {
    connection.invoke("ResetCount");
  });
};

connection.on("newData", function (time, speed, count) {

  // This subtract causes the data to be placed
  // in the centre of the chart as it arrives, 
  // which I personally think looks better...
  var dateValue = moment(time).subtract(5, 'seconds');

  speedValue.innerText = speed;
  countValue.innerText = count;

  // append the new data to the existing chart data
  myChart.data.datasets[0].data.push({
    x: dateValue,
    y: speed
  });

  // update chart datasets keeping the current animation
  myChart.update({
    preservation: true
  });
});
