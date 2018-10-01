<?php require_once "../config.php" ?>
<?php require_once "../database.php" ?>
<?php 

$database = new Database($DB_HOST, $DB_USER, $DB_PASS);
$database->connect_db();
$database->choose_database_name($DB_NAME);

//http://www.casinoomania.dev/page-api/users-api.php?action=update_user&id=1&username=hammy&facebookid=1&md5_string=a7c55fa63e50b4390058bacaccc58142
$mRequest = $_GET;
ksort($mRequest);
$mString = "";
foreach($mRequest as $key => $value) {
	if (strcmp($key,"md5_string") != 0) {
		if ($num == 0) {
			$mString .= $key."=".$value;
		} else {
			$mString .= "&".$key."=".$value;
		}
		$num++;
	}
}
$mString .= $SECRET_KEY;

$server_md5_string = md5($mString);
$client_md5_string = $_REQUEST['md5_string'];

$textarea = $mString."\n\n".$server_md5_string;

if (strcmp($server_md5_string, $client_md5_string) == 0) {
	//http://www.casinoomania.dev/page-api/users-api.php?action=get_user&id=1&md5_string=e03412d35062e37dbe954f9b95c497c6
	if (strcmp($_REQUEST['action'],"get_user") == 0) {
		$sql = "SELECT * FROM users WHERE id=%d;";  
		$query = sprintf($sql, $_REQUEST['id']);  
		$result = $database->query($query);
		print json_encode ($result);

	} else if (strcmp($_REQUEST['action'],"update_user") == 0) {
	}
} else {
	$error[0]['server_md5_string'] = $server_md5_string;
	$error[0]['client_md5_string'] = $client_md5_string;
	$error[0]['error_message'] = "server and client md5 string not the same! Fake Request!";
	print json_encode ($error);
}
    
?>

<!-- 
<?php 

function getUser($id) {
	$sql = "SELECT * FROM users WHERE id=%d;";  
	$query = sprintf($sql, $username);  
	$result = $database->query($query);
	print json_encode ($result);
}
?>
 -->

<?php $database->close_db(); ?>