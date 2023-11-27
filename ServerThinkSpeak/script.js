window.onload = function () {
    cambiaModo('auto')
};

function cambiaModo(modo){
    if(modo == 'auto'){
        desactivarBotones()
        cambiaEstado("lampara_estado", "Automatico");
        cambiaEstado("ventilador_estado", "Automatico");
    }else
    {
        activarBotones();
    }
    onSubmitForm();
}

function cambiaEstado(inputId, estado) {
    // Cambia el valor del input oculto según el estado seleccionado
    document.getElementById(inputId).value = estado;
    onSubmitForm();
}

function onSubmitForm() {
    $.ajax({
        url: "estado.php",
        type: "post",
        data: $("#ivernForm").serialize(),
        success: function(resultado){
            $("#resultado").html(resultado);
        }
    })
}

function desactivarBotones() {
    // Obtener la referencia a la tabla
    var tabla = document.getElementById('tablaBotones');

    // Verificar si la tabla existe
    if (tabla) {
        // Obtener todos los botones dentro de la tabla
        var botones = tabla.getElementsByTagName('button');

        // Iterar sobre los botones y aplicar el estilo
        for (var i = 0; i < botones.length; i++) {
            botones[i].classList.add('boton-desactivado');
        }
    } else {
        console.error('No se encontró la tabla con el id "tablaBotones"');
    }
}

function activarBotones() {
    // Obtener la referencia a la tabla
    var tabla = document.getElementById('tablaBotones');

    // Verificar si la tabla existe
    if (tabla) {
        // Obtener todos los botones dentro de la tabla
        var botones = tabla.getElementsByTagName('button');

        // Iterar sobre los botones y aplicar el estilo
        for (var i = 0; i < botones.length; i++) {
            botones[i].classList.remove('boton-desactivado');
        }
    } else {
        console.error('No se encontró la tabla con el id "tablaBotones"');
    }
}

/* document.getElementById('light-on').addEventListener('click', function () {
    var ts_api_key = document.getElementById('ts-api-key').value;
    var url = "http://api.thingspeak.com/update?api_key=" + ts_api_key + "&field1=1"
    $.getJSON(url, function (data) {
        console.log(data);
    });
});

document.getElementById('light-off').addEventListener('click', function () {
    var ts_api_key = document.getElementById('ts-api-key').value;
    var url = "http://api.thingspeak.com/update?api_key=" + ts_api_key + "&field1=0"
    $.getJSON(url, function (data) {
        console.log(data);
    });
});

function getThingSpeakLightState() {
    var ts_channel_id = document.getElementById('ts-channel-id').value;
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var myObj = JSON.parse(this.responseText);
            if (myObj.field1 == 1) {
                document.getElementById("LIGHTState").innerHTML = "Encendido";
                document.getElementById("LightImage").src = " data:image/png;base64,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";
            }
            else {
                document.getElementById("LIGHTState").innerHTML = "Apagado";
                document.getElementById("LightImage").src = " data:image/png;base64,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";
            }
        }
    };
    xhttp.open("GET", "https://api.thingspeak.com/channels/" + ts_channel_id + "/fields/1/last.json?api_key=YIFS5N23NKEK9VZW&results=2", true);
    xhttp.send();
}
 */