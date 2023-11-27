<?php
$host = "localhost";
$db = "id21371168_invernadero";
$user = "id21371168_root";
$password = "W3b0s1.0";

try {
    $dsn = "mysql:host=$host;dbname=$db";
    $pdo = new PDO($dsn, $user, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    echo '<script>alert("Conexión fallida");</script>';
    die();
}

// Consulta SQL para obtener los final
$query = 'SELECT lampara, ventilador
          FROM final
          ORDER BY id_final DESC LIMIT 1';
$statement = $pdo->prepare($query);
$statement->execute();
$actual = $statement->fetchAll(PDO::FETCH_ASSOC);
if (!empty($actual)) {
    $respuesta = array('actual' => $actual);
    echo json_encode($respuesta);
} else {
    echo json_encode(array('actual' => []));
}

?>