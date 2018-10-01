<?php 
//**************************************************
// Input into transaction table
//**************************************************
function inputTransaction($database, 
  $type, 
  $pid, 
  $nama_produk, 
  $bahan_barang,
  $supplier,
  $harga_beli,
  $harga_jual,
  $units,
  $cash_out,
  $cash_in,
  $profit,
  $writer,
  $pembeli) {
  
  if ($type == "beli") {
    $harga_jual = "-";
    $cash_in = "-";
    $asset = $cash_out;
  } else if ($type == "jual") {
    $harga_beli = "-";
    $cash_out = "-";
    
    $sql = "SELECT harga_per_unit FROM master_produk WHERE id=%d";
    $query = sprintf($sql, $database->escape($pid));
    $result = $database->query($query);
    foreach ($result as $object) {
      $harga_per_unit = $object->harga_per_unit;
      $profit = $cash_in - ($harga_per_unit * $units);
      $asset = $harga_per_unit * $units;
    }
  }
  
  $sql = "INSERT INTO transaction (
            pid,
            type, 
            nama_produk, 
            bahan_barang, 
            supplier, 
            harga_beli, 
            harga_jual, 
            units, 
            asset,
            cash_out, 
            cash_in, 
            profit, 
            writer,
            pembeli
          ) VALUES (
            %d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s'
          );";
  $query = sprintf($sql,
             $database->escape($pid),
             $database->escape($type),
             $database->escape($nama_produk),
             $database->escape($bahan_barang),
             $database->escape($supplier),
             $database->escape($harga_beli),
             $database->escape($harga_jual),
             $database->escape($units),
             $database->escape($asset),
             $database->escape($cash_out),
             $database->escape($cash_in),
             $database->escape($profit),
             $database->escape($writer),
             $database->escape($pembeli)
           );
  $result = $database->execute($query);
}

//**************************************************
// input into produk from transaction
//**************************************************
function updateProdukFromTransaction($database, $type, $pid, $transaction_units, $transaction_asset) {
  $sql = "SELECT * FROM master_produk WHERE id=%d";
  $query = sprintf($sql, $database->escape($pid));
  $result = $database->query($query);
  
  $current_asset = 0;
  $current_units = 0;
  $current_harga_per_unit = 0;
  foreach($result as $object) {
    $current_asset = $object->asset;
    $current_units = $object->units;
    $current_harga_per_unit = $object->harga_per_unit;
  }
  
  if ($type == 'beli') {
    $final_asset = $current_asset + $transaction_asset;
    $final_units = $current_units + $transaction_units;
    $harga_per_unit = $final_asset / $final_units;
  } else if ($type == 'jual') {
    $final_asset = $current_asset - $transaction_units * $current_harga_per_unit;
    $final_units = $current_units - $transaction_units;
    $harga_per_unit = $current_harga_per_unit;
  }
  
  $sql = "UPDATE master_produk SET asset='%s', units='%s', harga_per_unit='%s' WHERE id=%d";
  $query = sprintf($sql,
            $database->escape($final_asset),
            $database->escape($final_units),
            $database->escape($harga_per_unit),
            $database->escape($pid));
  $result = $database->execute($query);
}

//**************************************************
// input into transaction_gudang_relationship table
//**************************************************
function inputTransactionGudangTable($database, $tid, $gudang_keys, $type, $gudang_values, $writer) {
  for ($i=0; $i<count($gudang_values); $i++) {
    //print $gudang_values[$i];
    //print $gudang_keys[$i];
    $gid = $gudang_keys[$i];
    $unit = $gudang_values[$i];
    $sql = "INSERT INTO transaction_gudang_relationship (
              tid, gid, type, units, writer
            ) VALUES (
              %d, %d, '%s', '%s', '%s'
            );";
    $query = sprintf($sql, 
               $database->escape($tid),
               $database->escape($gid),
               $database->escape($type),
               $database->escape($unit),
               $database->escape($writer)
             );
    $result = $database->execute($query);
  }
}

//**************************************************
// Update Produk Gudang Relationship table
//**************************************************
function updateProdukGudangTable(
    $database,
    $tid,
    $pid, 
    $type, 
    $gudang_keys, 
    $gudang_values, 
    $writer) {
  for ($i=0; $i<count($gudang_values); $i++) {
    $key = $gudang_keys[$i];
    $val = $gudang_values[$i];
    //id in produk_gudang_relationship is always prod_id-gudang-id    
    $add_unit = $val;

    // check if there is additional unit!
    
    $stock_unit = 0;
    $sql = "SELECT * FROM produk_gudang_relationship WHERE produk_id=%d AND gudang_id=%d;";
    $query = sprintf($sql, 
              $database->escape($pid),
              $database->escape($key)
            );
    $result = $database->query($query);
    foreach($result as $object) {
      $stock_unit = $object->units;
    }
    
    if ($type == 'beli') {
      $units = $add_unit + $stock_unit;
    } else if ($type == 'jual') {
      $units = $stock_unit - $add_unit;
    }
    
    $sql = "UPDATE produk_gudang_relationship SET units='%s' WHERE produk_id=%d AND gudang_id=%d;";
    $query = sprintf($sql,              
              $database->escape($units),
              $database->escape($pid),
              $database->escape($key)
            );
    $result = $database->execute($query);
    if (mysql_affected_rows() <= 0) {
      $sql = "SELECT count(produk_id) as mcount FROM produk_gudang_relationship WHERE produk_id=%d AND gudang_id=%d;";
      $query = sprintf($sql,
              $database->escape($pid),
              $database->escape($key)
            );
      $result = $database->query($query);
      foreach($result as $object) {$count = $object->mcount;}
      
      if ($count <= 0) {
        $sql = "INSERT INTO produk_gudang_relationship (produk_id, gudang_id, units) VALUES ('%s', '%s', '%s');";
        $query = sprintf($sql,
                $database->escape($pid),
                $database->escape($key),
                $database->escape($units)
              );
        $result = $database->execute($query);
      }
    }    
  }
}

function getTransactionListResult($database, $sql, $where_clause, $query) {
}
?>