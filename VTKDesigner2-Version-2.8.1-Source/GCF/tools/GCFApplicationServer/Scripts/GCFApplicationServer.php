<?php
if( $HTTP_RAW_POST_DATA )
{
    $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    if($socket == false)
    {
        echo "Could not connect to GCFApplicationServer";
    }
    else
    {
        $address = "127.0.0.1";
        $port = 4232;
        $result = socket_connect($socket, $address, $port);
        if($result == false)
        {
            echo "Could not connect to GCFApplicationServer";
        }
        else
        {
        	header("Content-type: text/xml");
            socket_write($socket, $HTTP_RAW_POST_DATA, strlen($HTTP_RAW_POST_DATA));

            $bytesWritten = 0;
            while(1)
            {
                $out = socket_read($socket, 4096);
                if( strlen($out) == 0 || socket_last_error($socket) )
                    break;

                echo $out;
            }

            socket_close($socket);
        }
    }
}
?>

