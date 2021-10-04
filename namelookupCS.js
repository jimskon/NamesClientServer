// JavaScript for Name Data Lookup Demo
// Jim Skon, Kenyon College, 2021
var searchType;  // Save search type here

console.log("Start!");
searchType="Last";
// Add a click event for the search button
document.querySelector("#search-btn").addEventListener("click", (e) => {
    e.preventDefault()
    getMatches();
});

// Add event for enter on search                                                                                                    
var search = document.getElementById("search");
// Respond to enter key
search.addEventListener("keyup", function(event) {
    event.preventDefault()
    
    // Number 13 is the "Enter" key on the keyboard
    if (event.keyCode === 13) {
        // Cancel the default action, if needed
        event.preventDefault();
        // Trigger the button element with a click
        getMatches();
    }
});


// Add an event listener for each item in the pull down menu
document.querySelectorAll('.dropdown-menu a').forEach(item => {
    item.addEventListener('click', event => {
	element = event.target;
	searchType=element.textContent;
	console.log("pick: "+searchType);
	// Get the pulldown parent
	pullDown = element.parentElement.parentElement;
	// Get and set the selection displayed
	selection = pullDown.querySelectorAll(".selection")[0];
	selection.innerHTML = searchType;
	
    })
})

// Build output table from comma delimited list
function nameTable(data) {
    var table = '<table class="w3-table-all w3-hoverable" border="2"><tr><th>Name</th><th>%</th><th>Rank</th><tr>';
    d = data["results"];
    
    for (var i = 0; i < d.length; i++) {
	table += "<tr><td>"+d[i]["name"]+"</td><td>"+d[i]["percent"]+"</td><td>"+d[i]["rank"]+"</td></tr>";
    }
    table += "</table>";

    return table;
}


function processResults(results) {
    document.querySelector('#searchresults').innerHTML = nameTable(results);
}

function clearResults() {
    document.querySelector('#searchresults').innerHTML = "";
}

function getMatches(){
    console.log("getMatches!");
    var searchStr=document.querySelector('#search').value;
    console.log(searchStr+":"+searchType);

    // Ignore short requests
    if (searchStr.length < 2) return;

    // Clear the previous results
    document.querySelector('#searchresults').innerHTML = "";

    fetch('/cgi-bin/skon_namelookupCS.cgi?name='+searchStr+'&type_select='+searchType, {
	method: 'get'
    })
	.then (response => response.json() )
        .then (data => processResults(data))
	.catch(error => {
	    {alert("Error: Something went wrong:"+error);}
	})
}


