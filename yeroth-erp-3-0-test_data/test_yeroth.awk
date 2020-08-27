BEGIN{
		FS = ";"
		OFS = ";"
}

//{
		libelle = $3
		
		a_length = split(libelle, a, " ")
		
		categorie = substr(a[1], 2, length(a[1]))

		print $1 ";" $2 ";\"" categorie "\";" $3
}


END{
}
