var temp = document.getElementById("temp");
var humidity = document.getElementById("humd");

function weather(){
  fetch("/request", {
      method: "POST",
      headers: {
          'Accept': 'application/json',
          'Content-Type': 'text/plain'
      },
      body: ""
  }).then(res => {
      return res.json();
  }).then(weather_data => {
      temp.innerHTML = String(parseInt(weather_data.temp)) + "&deg;C";
      humidity.innerHTML = String(parseInt(weather_data.humidity)) + "&percnt;";
  }).catch(e => {
      temp.innerHTML = "Err";
      humidity.innerHTML = "Err";
  });
}
weather();

setInterval(() => {
  weather();
}, 5000);