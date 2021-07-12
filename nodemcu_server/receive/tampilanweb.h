const char MAIN_page[] PROGMEM = R"=====(

<!doctype html>
<html>

<head>
  <title>Monitoring Air Danau</title>
  <!--For offline ESP graphs see this tutorial https://circuits4you.com/2018/03/10/esp8266-jquery-and-ajax-web-server/ -->
  <script src = "https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.3/Chart.min.js"></script>  
  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css" integrity="sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2" crossorigin="anonymous">
  <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js" integrity="sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj" crossorigin="anonymous"></script>
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ho+j7jyWK8fNQe+A12Hb8AhRq26LrZ/JpcUGGOn+Y7RsweNrtN/tE3MoK7ZeZDyx" crossorigin="anonymous"></script>
  
  
  <style>
    @import url('https://fonts.googleapis.com/css2?family=Hind+Siliguri:wght@400;600&display=swap');
    h3{
      font-family: 'Hind Siliguri', sans-serif;
    }
  canvas{
    -moz-user-select: none;
    -webkit-user-select: none;
    -ms-user-select: none;
  }

  /* Data Table Styling */
  #dataTable {
    font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
    border-collapse: collapse;
    width: 100%;
  }

  #dataTable td, #dataTable th {
    border: 1px solid #ddd;
    padding: 8px;
  }

  #dataTable tr:nth-child(even){background-color: #f2f2f2;}

  #dataTable tr:hover {background-color: #ddd;}

  #dataTable th {
    padding-top: 12px;
    padding-bottom: 12px;
    text-align: left;
    background-color: #4CAF50;
    color: white;
  }
  .tables{
    padding: 2em 2em ;
  }

  html {
    position: relative;
    min-height: 100%;
}

body {
    margin: 0 0 100px;
    /* bottom = footer height */
    padding: 25px;
}

footer {
    padding: 1em 1em;
    background-color: rgb(236, 235, 233);
    position: absolute;
    left: 0;
    bottom: 0;
    height: 4em;
    width: 100%;
    overflow: hidden;
}
  </style>
</head>

<body>

  <div class="text-center mt-4">
    <h3>
      Rancang Bangun Pengukur Kekeruhan Air Danau
    </h3>
    <h3>
      menggunakan telemetri dengan sumber solar cell
    </h3>
    <p> Monitoring Air Danau </p>
  </div>

  <div class="container">
    <div>
      <h4 class="ml-3 mt-4">
        Graph
      </h4>
    </div>
    <div class="card shadow mt-3">    
      <div class="chart-container" position: relative; height:350px; width:100%">
        <canvas id="Chart1" width="400" height="400"></canvas>
      </div>
    </div>
    
    <div class=" mt-4">
      <h4 class="ml-3">
        Table
      </h4>
    <div>

    <div class="card shadow mt-3">
      <div class="tables">
        <table id="dataTable">
          <tr>
            <th>Waktu</th>
            <th>PH</th>
            <th>Kekeruhan</th>
          </tr>
        </table>
      </div>
    </div>
  </div>

  <footer>
    <div class="text-center">
      <p> &copy;copyright 2021 | Rizki giali R. </p>
    </div>
  </footer> 
<script>
//Graphs visit: https://www.chartjs.org
var PH = [];
var Kekeruhan = [];

var timeStamp = [];
var Chart1;

function showGraph1()
{

    var ctx = document.getElementById("Chart1").getContext('2d');
    Chart1 = new Chart(ctx, {
        type: 'line',
        data: {
            labels: timeStamp,  //Bottom Labeling
            datasets: [{
                label: "PH",
                fill: false,  //Try with true
                backgroundColor: 'rgba( 243, 156, 18 , 1)', //Dot marker color
                borderColor: 'rgba( 243, 156, 18 , 1)', //Graph Line Color
                data: PH,
            },
            {
                label: "Kekeruhan",
                fill: false,  //Try with true
                backgroundColor: 'rgba( 6, 156, 18 , 1)', //Dot marker color
                borderColor: 'rgba( 6, 156, 18 , 1)', //Graph Line Color
                data: Kekeruhan,
            }],
        },
        options: {
            title: {
                    display: true,
                    text: "Sensor"
                },
            maintainAspectRatio: false,
            elements: {
            line: {
                    tension: 0.5 //Smoothening (Curved) of data lines
                }
            },
            scales: {
                    yAxes: [{
                        ticks: {
                            beginAtZero:true
                        }
                    }]
            }
        }
    });
}

//On Page load show graphs
window.onload = function() {
  console.log(new Date().toLocaleTimeString());
  showGraph1();
 
};

//Ajax script to get ADC voltage at every 5 Seconds 
//Read This tutorial https://circuits4you.com/2018/02/04/esp8266-ajax-update-part-of-web-page-without-refreshing/

setInterval(function() {
  // Call a function repetatively with 5 Second interval
  getData();
}, 1000); //5000mSeconds update rate
 
function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
     //Push the data in array
  var time = new Date().toLocaleTimeString();
  var str = this.responseText; 
  var res = str.split(";");
  
  //var ADCValue = res[1];
    PH.push(res[0]);
    Kekeruhan.push(res[1]);
    
    
      timeStamp.push(time);
      window.Chart1.update();

  //Update Data Table
    var table = document.getElementById("dataTable");
    var row = table.insertRow(1); //Add after headings
    var cell1 = row.insertCell(0);
  
    var cell2 = row.insertCell(1);
    var cell3 = row.insertCell(2);

  
    cell1.innerHTML = time;
  
    cell2.innerHTML = res[0];
    cell3.innerHTML = res[1];

    }
  };
  xhttp.open("GET", "readADC", true); //Handle readADC server on ESP8266
  xhttp.send();
}
    
</script>
</body>

</html>

)=====";
