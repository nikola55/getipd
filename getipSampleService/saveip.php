<?php
require_once('config.php');

$authenticated = FALSE;

$date = date('d-m-Y H:i:s');


if (!isset($_SERVER['PHP_AUTH_USER'])) 
{
    header('WWW-Authenticate: Basic realm="My Realm"');
    header('HTTP/1.0 401 Unauthorized');
    die($date.": 0=Unauthorized");
} 
else 
{
    $authenticated = $_SERVER['PHP_AUTH_USER'] === $username &&
					 $_SERVER['PHP_AUTH_PW'] === $password;
	
}

if(!$authenticated) 
{
	die($date.": 0=Invalid credentials");
}

if(isset($_POST["ip_address"]))
{

	$conn=mysqli_connect($mysql_host, $mysql_user, $mysql_pass, $database);

	if(mysqli_connect_errno())
	{
		die($date.": 0=Connection to database failed: ".mysqli_connect_error());
	}
	
	$query = "UPDATE ip_logs SET ip='".$_POST["ip_address"]."', last_reported = NOW();";
	
	$res = mysqli_query($conn, $query);
	
	if ($res) 
	{
		die($date.": 1=Reported");
	}
	else
	{
		die($date.": 0=Error occurred: ".mysql_error());
	}
	
	mysqli_close($conn);
	
}
else
{
	die($date.": 0=No ip address in the input map.");
}