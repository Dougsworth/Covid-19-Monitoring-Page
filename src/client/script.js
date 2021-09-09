function generateTableRow(id){
    // Create the new row to be added to the table
    var tableRow = document.createElement("TR");
    tableRow.classList.add("table-row");

    //Populate the elements in the row
    var tdID = document.createElement("TD");
    tdID.innerHTML = id;

    var tdLocation = document.createElement("TD");
    tdLocation.innerHTML = "Engineering";

    var tdTemperature = document.createElement("TD");
    tdTemperature.innerHTML = "28.5";

    var tdStatus = document.createElement("TD");
    tdStatus.innerHTML = "Normal";

    var tdDate = document.createElement("TD");
    tdDate.innerHTML = "August 26,2021";

    var tdTime = document.createElement("TD");
    tdTime.innerHTML = "3:59";

    tableRow.append(tdID);
    tableRow.append(tdLocation);
    tableRow.append(tdTemperature);
    tableRow.append(tdStatus);
    tableRow.append(tdDate);
    tableRow.append(tdTime);
    
    return tableRow;
}

var tableBody = document.querySelector(".table-body");
var IDS = ["11", "12", "13", "14", "15"];

IDS.forEach(id => {
    tableBody.append(generateTableRow(id));
});