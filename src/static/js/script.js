const serverURL = document.domain;
const port = location.port;

function generateTableRow(data){
    // Create the new row to be added to the table
    var tableRow = document.createElement("TR");
    tableRow.classList.add("table-row");

    //Populate the elements in the row
    var tdLocation = document.createElement("TD");
    tdLocation.innerHTML = "Engineering";

    var tdTemperature = document.createElement("TD");
    tdTemperature.innerHTML = data.temperature;

    var tdStatus = document.createElement("TD");
    tdStatus.innerHTML = data.status;

    var tdDate = document.createElement("TD");
    tdDate.innerHTML = data.day +"/"+ data.month +"/"+ data.year;

    var tdTime = document.createElement("TD");
    tdTime.innerHTML = data.hour +":"+ data.minute +"."+ data.second;

    tableRow.append(tdLocation);
    tableRow.append(tdTemperature);
    tableRow.append(tdStatus);
    tableRow.append(tdDate);
    tableRow.append(tdTime);
    
    return tableRow;
}

var dataPath = "http://" + serverURL + ":"+ port +"/api/data";
function getTableData(){
    console.log(dataPath);
    return fetch(dataPath).then(res => res.json()).then(json => json);
}

async function displayTableData(){
    let rowData = await getTableData();  
    console.log(rowData);

    rowData.forEach(row => {
        //console.log(patient.p_username);
        var content = document.querySelector(".table-body");
        content.append(generateTableRow(row));        
    });
}

window.onload = function(){
    displayTableData();
}