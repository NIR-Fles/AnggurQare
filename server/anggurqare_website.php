<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>AnggurQare Website</title>
  <meta http-equiv="refresh" content="5"> <!-- Refresh the page every 5 seconds -->
</head>
<body>
  <table>
    <tr>
      <th>Id</th>
      <th>Soil pH</th>
      <th>Soil Moisture</th>
      <th>Nutrition</th>
      <th>Water pH</th>
      <th>Water Temperature</th>
      <th>Water Level</th>
      <th>Temperature</th>
      <th>Lightness</th>
      <th>Humidity</th>
    </tr>
    <?php
      include("anggurqare_connection.php");

      $sql = "SELECT id, soil_ph, soil_moisture, nutrition_tds, water_ph, water_temp, water_level, temperature, lightness, humidity FROM anggurqare_log ORDER BY id DESC LIMIT 1";
      $result = $conn->query($sql);

      if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        echo "<tr><td>". $row["id"] ."</td><td>". $row["soil_ph"] ."</td><td>". $row["soil_moisture"]."</td><td>". $row["nutrition_tds"]."</td><td>". $row["water_ph"]."</td><td>". $row["water_temp"]."</td><td>". $row["water_level"]."</td><td>". $row["temperature"]."</td><td>". $row["lightness"]."</td><td>". $row["humidity"]."</td></tr>";
      } else {
        echo "<tr><td colspan='10'>No data available</td></tr>";
      }

      $conn->close();
    ?>
  </table>
</body>
</html>
