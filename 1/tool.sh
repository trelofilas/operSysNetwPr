#ANTONIOS THEOFILOU 
#AM:5300 

#!/bin/bash  

insertfile=0;	
firnam="FALSE";
lasnam="FALSE";
bros="FALSE";

if [ "$#" -eq  0 ]; then   #an h prwth parametros den uparxei tote prokeitai gia thn arxikh periptwsh  
	echo "5300"
	exit;
fi

parameters="$#";		#krataw ton ari8mo twn parametrwn gia na to xrhsimopoihsw argotera
key="$1";				#arxikopoihsh ths key sthn prwth parametro.
until [ $# -eq 0 ] ; do	#8elw na exw apo 2-6 parametrous kai panw gia ola ta erwthmata 
	case $key in 		#diakrinw periptwseis parametrwn eisodou
	-f)					#elegxos gia parametro -f		
		insertfile="$2"	#h parametros -f akolou8eitai apo <file>
		shift;			#kanw olis8hsh sthn epomenh parametro giati dinw apo 
						#monos mou timh sthn epomenh parametro apo thn prwth  (1)
		;;
	-id)				#elegxos gia parametro -id
		id1="$2"		#h parametros -id akolou8eitai apo <id>
		shift;			#isxei oti sto (1)
		;;
	--born-since)		#elegxos gia parametro -born-since
		dateA="$2"		#h parametros --born_since akolou8eitai apo <dateA>
		shift;			#isxei oti sto isxei oti sto (1)
		;;
	--born-until)		#elegxos gia parametro -born-until
		dateB="$2"		#h parametros --born_until akolou8eitai apo <dateB>
		shift;			#isxei oti sto (1)
		;;	
	--firstnames)		#elegxos gia parametro --firstnames
		firnam="TRUE"	#otan parw eisodo thn parametro --firstnames odhgw sthn metablhth 
						#firnam thn timh TRUE etsi wste na kserw oti prokeitai gia to
						#3o erwthma kai argotera na to xeiristw katallhla
		;;
	--lastnames)		#elegxos gia parametro --lastnames
		lasnam="TRUE"	#otan parw eisodo thn parametro --lastnames odhgw sthn metablhth 
						#lasnam thn timh TRUE etsi wste na kserw oti prokeitai gia to
						#4o erwthma kai argotera na to xeiristw katallhla
		;;
	--browsers)			#elegxos gia parametro --browsers
		bros="TRUE"		#otan parw eisodo thn parametro --browsers odhgw sthn metablhth 
						#bros thn timh TRUE etsi wste na kserw oti prokeitai gia to
						#6o erwthma kai argotera na to xeiristw katallhla
		;;
	--edit)				#elegxos gia parametro --edit
						#h parametros --edit akolou8eitai apo <id> <column> <value>
		id2="$2"		#h id2 8a parei thn timh tou dwsmenou id
		column="$3"		#h column 8a prei thn timh ths sthlhs pou 8eloume na ginei h allagh
		value="$4"		#h value 8a parei thn timh pou 8eloume na allaksoume
		shift 3;		#kanw olis8hsh kata 3 parametrous giati dinw times stis 3 epomenes parametrous  
		;;
	esac;
	shift;				#kanw olis8hsh kata mia 8esh gia na proxwrhsei to programma sthn epomenh parametro
	key="$1"			#allagh timhs ths key wste na exei thn timh ths prwths parametrou pali
done					#telos ths while

if [ -s "$insertfile" ]; then	#an exoume parei san eisodo arxeio opote insertfile uparxei 
								#kai einai megalutero se mege8os apo 0 (-s)
								#tote h eisodos mas einai swsth kai mporoume na paroume tis 
								#periptwseis twn erwthmatwn.

	#gia oles tis awk pou xrhsimopoiountai oi entoles (-F "|" kai !/^#/ einai gia na mh lhfthoun upopsh 
	#grammes pou ksekinan me #,ta sxolia dhladh alla kai na allaksei to default diaxwristiko pediwn 
	#apo to keno pou einai se | )
	if [ "$parameters" -eq 2 ]; then	#an o ari8mos twn parametrwn einai isos me 2 tote	
		awk -F "|" '!/^#/{print}' $insertfile; 	#me thn awk ektupwnetai to periexomeno olou to arxeio
		exit;	#exodos apo to programma
	fi

	if [ -n "$id1" ]; then	#an uparxei h metablhth id1 tote prokeitai gia to deutero erwthma ths askhshs
		searchId="$id1"	#h meteblhth searchId pairnei thn timh ths id1
		awk -F "|" -v sId="$searchId" '!/^#/ { if($1 == sId) printf("%s %s %s\n",$3,$2,$5)}' $insertfile ;	
		#h awk psaxnei an se kapoia grammh to 1o pedio isoutai me to sId to opoio isoutai 
		#me thn searchId kai ektupwnei ta pedia firstnames(3),lastnames(2),birthday(5)
		#pou einai kai ta zhtoumena me ena keno anamesa tous
		exit;	#exodos apo to programma
	elif [ "$firnam" = "TRUE" ]; then	#an h metablhth exei thn timh ayth tote eimaste sto 3o erwthma	
		awk -F "|" '!/^#/ { print $3 }' $insertfile | sort | uniq ;
		#h prwth awk briskei ola ta firstnames,epeita taksinomountai ap th sort,
		#kai h uniq krataei mono ta monadika
		exit; 	#exodos apo to programma
	elif [ "$lasnam" = "TRUE"  ]; then	#an h metablhth exei thn timh ayth tote eimaste sto 4o erwthma
		awk -F "|" '!/^#/ { print $2 }' $insertfile | sort | uniq ;
		#h prwth awk briskei ola ta lasttnames,epeita taksinomountai ap th sort,
		#kai h uniq krataei mono ta monadika 
		exit;	#exodos apo to programma
	elif [ -n "$dateA" ] && [ -n "$dateB" ]; then	#h periptwsh pou 8a do8ei hmeromhnia apo mexri
		awk -F "|" -v dA="$dateA" -v dB="$dateB" '!/^#/ { if( dA <= $5 && dB >= $5 ) print }' $insertfile; 
		#h awk briskei ths hmeromhnies apo to 5o pedio birhtdays pou anhkoun mesa sto diasthma 
		#pou 8eloume kai ektypwnei tis grammes autes
		exit; 	#exodos apo to programma
	elif [ -n "$dateA" ]; then		#h periptwsh pou 8a do8ei hmeromhnia apo
		awk -F "|" -v dA="$dateA" '!/^#/ { if( dA <= $5 ) print }' $insertfile;
		#h awk briskei ths hmeromhnies apo to 5o pedio birhtdays pou einai megaluteres apo thn dwsmenh hmeromhnia
		exit; 	#exodos apo to programma
	elif [ -n "$dateB" ]; then		#h periptwsh pou 8a do8ei hmeromhnia mexri
		awk -F "|" -v dB="$dateB" '!/^#/ { if( dB >= $5 ) print }' $insertfile;
		#h awk briskei ths hmeromhnies apo to 5o pedio birhtdays pou einai mikroteres apo th dwsmenh hmeromhnia
		exit; 	#exodos apo to programma
	elif [ "$bros" = "TRUE" ]; then		#an h metablhth bros exei thn timh auth tote eimaste sto 6o erwthma
		awk -F "|" '!/^#/{ array[$8]++ } END{ for(i in array) printf( "%s %s\n",i,array[i] )  }' $insertfile | sort ; 
		#Orizw dynamika ena array (array[]) me megethos pou auksanetai gia kathe diaforetiko pedio bros($8)
		#Gia ka8e browser, auksanetai o metrhths ston array . Sto telos twn eggrafwn
		#mesw ths END, ektypwnontai oloi oi browsers i ka8ws kai to plh8os emfanisewn array[i].
		#Ta apotelesmata taksinomountai alfarhm8htika mesw ths sort
		exit; 	#exodos apo to programma
	elif [ -n "$id2" ] && [ -n "$column" ] && [ -n "$value" ]; then
		idd="$id2"	#h idd pairnei thn timh ths id2
		colm="$column"	#h colm pairnei thn timh ths column
		val1="$value"	#h val1 pairnei thn timh ths value
		line=$(sed -n '/^'$idd"|"'/=' $insertfile;)
		#h line pairnei thn timh apo thn sed,h opoia upologizei thn grammh sthn opoia einai
		#to dwsmeno id.To (-n) mpainei etsi wste na do8ei sthn line mono h timh pou ginetai h
		#h tautish me ta dedomena pou dinwOson afora thn ekfrash ('/^'idd"|"'/=') pou brisketai 
		#mesa sth sed eksasfalizei oti psaxoume string pou na arxizei(^) me thn idd akolou8oumenh 
		#apo ("|") pou einai to diaxwristiko twn pediwn.
		if [ "$colm" -lt 2 ] || [ "$colm" -gt 8 ]; then
		#an h dwsmenh timhgia sthlh den einai apodekth termatizetai to programma
			echo "Not acceptable column value..."
			exit;		#exodos apo to programma
		elif [ -z "$line" ]; then
			#an h line den exei parei kapoia timh apo nwrutera to programma termatizetai
			echo "Not acceptable id..."
			exit;		#exodos apo to programma
		else
			awk -F "|" -v sId="$idd" -v col="$colm" -v val="$val1" ' $1 == sId {$col=val}1 ' OFS="|" $insertfile > tmp1.dat  && mv tmp1.dat $insertfile ; 
		#dhlwnw tis metablhtes sthn awk.Epeita psaxnei sto prwto pedio to dwsmeno id
		#otan to brei bazei thn timh pou 8eloume sthn sthlh(pedio) pou 8eloume.
		#ta apotelesmata auta perniountai se ena temp arxeio kai parallhla to 
		#temp arxeio me tis allagmenes times antika8ista to arxiko dwsmeno arxeio.
		#se auto to shmeio h duskolia pou sunanthsa htan na sumperilavw sto teliko epeksergasmeno arxeio
		#ta diaxwristika sthn grammh pou egine h allagh me ton tropo me ton opoio ulopoihsa thn awk...
		#lu8hke telika! meta apo apeira peiramata me mia mono entolh (OFS="|) sto telos ths awk.
			exit;	#exodos apo to programma
		fi	
	fi
else 
	echo "No file found..."		#an den do8ei arxeio eisodou tote ektupwnetai to mhnuma auto
	exit; 	#exodos apo to programma
fi    