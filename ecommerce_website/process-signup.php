<?php

if (empty($_POST["name"])) {
    die("Name is required");
}

if ( ! filter_var($_POST["email"], FILTER_VALIDATE_EMAIL)) {
    die("Valid email is required");
}

if (strlen($_POST["password"]) < 8) {
    die("Password must be at least 8 characters");
}

if ( ! preg_match("/[a-z]/i", $_POST["password"])) {
    die("Password must contain at least one letter");
}

if ( ! preg_match("/[0-9]/", $_POST["password"])) {
    die("Password must contain at least one number");
}

if ($_POST["password"] !== $_POST["password_confirmation"]) {
    die("Passwords must match");
}

$password_hash = password_hash($_POST["password"], PASSWORD_DEFAULT);

$mysqli = require __DIR__ . "/database.php";
$email = $_POST["email"];

// Verificați dacă adresa de email există deja în baza de date
$sql_check = "SELECT * FROM user WHERE email = ?";
$stmt_check = $mysqli->prepare($sql_check);
$stmt_check->bind_param("s", $email);
$stmt_check->execute();
$result_check = $stmt_check->get_result();

if ($result_check->num_rows > 0) {
    die("Email already taken");
}

// Inserați utilizatorul în baza de date
$sql_insert = "INSERT INTO user (name, email, password_hash)
               VALUES (?, ?, ?)";

$stmt_insert = $mysqli->prepare($sql_insert);

if (!$stmt_insert) {
    die("SQL error: " . $mysqli->error);
}

$stmt_insert->bind_param("sss", $_POST["name"], $email, $password_hash);

if ($stmt_insert->execute()) {
    header("Location: signup-succes.html");
    exit;
} else {
    die($mysqli->error . " " . $mysqli->errno);
}
?>