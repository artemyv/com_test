$gorilla = New-Object -ComObject  Ape.Gorilla
$gorilla.EatBanana
$gorilla.SwingFromTree
$weight = $gorilla.Weight
Write-Host $weight kg

$gorilla.Hit(100);
