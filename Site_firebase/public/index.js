import { getDatabase, ref, set, child, onValue, get } from "https://www.gstatic.com/firebasejs/9.8.4/firebase-database.js";

const db = getDatabase();

const referencia = ref(db, "/");

function teste(){
  
  window.onload = () => {

    var nivel = []
    var peso = []
    var temp_alimento = []
    var temp_sensor1 = []
    var temp_sensor2 = []
    var umid_sensor1 = []
    var umid_sensor2 = []
    var tempo = [] //vetor de double


    get(child(referencia, "/")).then((snap) => {
      snap.forEach(value => {
        
        nivel.push(value.child("nivel").val())
        peso.push(value.child("peso").val())
        temp_alimento.push(value.child("temp_alimento").val())
        temp_sensor1.push(value.child("temp_sensor_1").val())
        temp_sensor2.push(value.child("temp_sensor_2").val())
        umid_sensor1.push(value.child("umid_sensor_1").val())
        umid_sensor2.push(value.child("umid_sensor_2").val())
        tempo.push(parseFloat(value.child("tempo").val()) * 1000)   
      
      });
    }).then( aaa => {

      var matriz = [[]], fetch = document.getElementById('fetch');
      var aux_date = []
      var humanDateFormat = []

      for(var i = 0; i < tempo.length; i++){
        aux_date[i] = new Date(tempo[i])
      }
      for(var i = 0; i < tempo.length; i++){
        humanDateFormat[i] = aux_date[i].toLocaleString()
      }

      for(var i = 0; i < tempo.length; i++){
        matriz.push([humanDateFormat[i], nivel[i], peso[i], temp_alimento[i], temp_sensor1[i], temp_sensor2[i], umid_sensor1[i], umid_sensor2[i]]);
      }
      matriz.push(['data' ,'nivel', 'peso', 'temperatura alimento', 'temperatura 1', 'temperatura 2', 'humidade 1', 'humidade 2']);

      var count = 0;

      for(var i=matriz.length - 1; i >= 0; i--){
        if(count == 50){
          break;
        }
        var newRow = fetch.insertRow(fetch.length);
        for(var j=0; j<matriz[i].length; j++){
            var cell = newRow.insertCell(j);
            cell.innerHTML = matriz[i][j];
        }
        count++;
      }

    });

  }
}

function download_xls(){

    var nivel = []
    var peso = []
    var temp_alimento = []
    var temp_sensor1 = []
    var temp_sensor2 = []
    var umid_sensor1 = []
    var umid_sensor2 = []
    var tempo = [] //vetor de double


    get(child(referencia, "/")).then((snap) => {
      snap.forEach(value => {
        
        nivel.push(value.child("nivel").val())
        peso.push(value.child("peso").val())
        temp_alimento.push(value.child("temp_alimento").val())
        temp_sensor1.push(value.child("temp_sensor_1").val())
        temp_sensor2.push(value.child("temp_sensor_2").val())
        umid_sensor1.push(value.child("umid_sensor_1").val())
        umid_sensor2.push(value.child("umid_sensor_2").val())
        tempo.push(parseFloat(value.child("tempo").val()) * 1000)   
      
      });
    }).then( aaa => {

      var matriz = [['data' ,'nivel', 'peso', 'temperatura alimento', 'temperatura 1', 'temperatura 2', 'humidade 1', 'humidade 2']], fetch = document.getElementById('fetch');
      var aux_date = []
      var humanDateFormat = []

      for(var i = 0; i < tempo.length; i++){
        aux_date[i] = new Date(tempo[i])
      }
      for(var i = 0; i < tempo.length; i++){
        humanDateFormat[i] = aux_date[i].toLocaleString()
      }

      for(var i = 0; i < tempo.length; i++){
        matriz.push([humanDateFormat[i], nivel[i], peso[i], temp_alimento[i], temp_sensor1[i], temp_sensor2[i], umid_sensor1[i], umid_sensor2[i]]);
      }

      let str = ""

      for(var i=0; i < matriz.length; i++){
        var j;
        for(j=0; j < matriz[i].length - 1; j++){
            str += matriz[i][j] + ";"
        }
        str += matriz[i][j] + "\n"
      }
      
      
      saveData(str, "Beehive.csv");
    });

}

function saveData (data, fileName) {
  var savingElement = document.createElement("a");
  
  document.body.appendChild(savingElement);
  savingElement.style = "display: none";
  var blob = new Blob([data], {type: "text/plain"})
  console.log(blob)
  var url = window.URL.createObjectURL(blob);
  console.log(url)
  savingElement.href = url;
  savingElement.download = fileName;
  console.log(savingElement)
  savingElement.click();
  window.URL.revokeObjectURL(url);
}


export  {teste, download_xls};