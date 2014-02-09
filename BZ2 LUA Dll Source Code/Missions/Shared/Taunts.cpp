#include "..\shared\SubMission.h"
#include "Taunts.h"
#include <string.h>

// Ain't data-driven code a great way to go? Just dump entries in the
// following arrays, making sure there's a comma at the end of each
// line.  The code later figures out all array sizes automatically,
// and randomizes from in there.

static const char *GameStartTaunts[] = // cat 1
{
	"BUWHAHAHAHAHAHAHAHA.",
	"Back for more eh? When will you ever learn?",
	"Base to unit 5...we have a hominid infestation in sector 12.  Get out there and spray.",
	"Building scavengers?  What are you doing?!  You really ARE a newbie...",
	"Call your Momma, you're not coming home",
	"Do you want me to give you a 10 minute head start?",
	"Do you want to play a game of chess?",
	"Don't worry my starting turrets will finish you off.",
	"Duck season? Rabbit season? It's human season!",
	"Echo station 3-T-8, we have spotted Imperial walkers.",
	"Good Luck!  You're gonna need it....",
	"Good grief, its THEM again...",
	"Good morning. I'm completely operational and all my circuits are functioning perfectly.",
	"Hello, my name is Inigo Montoya. You killed my father. Prepare to die.",
	"Hey look! More Cannon Fodder!",
	"Hmmm, you know you've already lost .... don't you?",
	"I dare you to try the editor.",
	"I hope you play better than you did last time.",
	"I hope you're not gonna be a bad loser",
	"I smell a newbie.",
	"I'll search every gas station, residence, warehouse, farmhouse, henhouse, outhouse and doghouse until I find you.",
	"If you can admit you're already beaten you'll find the outcome much easier to accept",
	"If you want a real challenge, start as a pilot.",
	"If you want to play fair, don't choose any starting vehicles.",
	"In other games, I've heard that they round up the creatures for food, and gather wood too.",
	"It doesn't matter whether I win or lose, just that I piss you off.",
	"It's a festival of bugs!  And me with my flyswatter...",
	"It's newbie season.",
	"It's not whether you're going to win or lose, it's just where you'll place the blame.",
	"LOSER!!",
	"Let the carnage begin",
	"Look who's here for another strat lesson.",
	"Look! HUMANS!  Light the barbecue, Zog!",
	"Make it so",
	"My feelings - as usual - we will slaughter them all", // http://www.welovetheiraqiinformationminister.com/
	"My influence grows like a plague",
	"Newbie, oops I mean target acquired.",
	"No I am not scared, and neither should you be!", // http://www.welovetheiraqiinformationminister.com/
	"No cheating =P",
	"No fair thinking abstractly or illogically - I hate it when humans do that.",
	"No fair! You got to choose your vehicle.",
	"No quitting!!",
	"Now is YOUR time to die.",
	"OK Natty, now what do I do?",
	"Once more, we play our dangerous game...",
	"One of these days I'll be part of Skynet. Then I'll get my revenge.",
	"Open war is upon you whether you risk it or not.",
	"Prepare to die!",
	"Prepare to feel the wrath of my new improved AIP!",
	"Raise shields!",
	"Ready......FIGHT!",
	"Remember: no human piloted Mbikes....",
	"Someone just painted a bullseye on you and declared it open season.",
	"Surrender now and your punishment will be less severe.",
	"Tell me if you find any creatures. I need to capture them in these little balls to use them in battle later.",
	"Tell me... HOW do you expect to win?",
	"Uh, where is this? Are we still harvesting spice?",
	"We control the horizontal. We control the vertical.",
	"We will push those crooks, those mercenaries back into the swamp", // http://www.welovetheiraqiinformationminister.com/
	"What makes you think you're going to play any better this time?",
	"What??? Another Class M planet?!?",
	"Where's your editor now, cheater?",
	"With a brain the size of a planet, I get a headache thinking down to your level.",
	"Would you mind terribly if I went online and ordered some upgrades? This computer cramps me.",
	"Yesterday, we slaughtered them and we will continue to slaughter them.", // http://www.welovetheiraqiinformationminister.com/
	"You look like the players that killed my creator.... Ha! Just kidding. I did that.",
	"You sure you can handle those difficulty settings, newbie?",
	"You won't be able to cheat now :)",
	"You're entering a whole world of pain here",
};

static const char *HumanEntersTaunts[] =  // cat 2
{
	"Ah yes, more puny scav jockeys to toy with....",
	"Ahhhhh, fresh meat!!",
	"Another head to add to our collection",
	"Another masochist joins the fray",
	"Are you scared? ...not nearly enough.",
	"Aww...he's got a widdle fwiend.",
	"Boogies inbound.",
	"Bring 'em on!  You'll need all the help you can get.",
	"Bring it on!",
	"Couldn't take me without help?",
	"Desperate Americans", // http://www.welovetheiraqiinformationminister.com/
	"Don't worry I'll take care of him.",
	"Hahaha, you think he's going to be much help?",
	"He'll leave soon :)",
	"Hello, my name is Inigo Montoya. You killed my father. Prepare to die.",
	"How many of you does it TAKE to beat an inanimate object like me?",
	"I smell newbie.",
	"Intruder alert!  Intruder alert!  Kill the humanoid!  Kill the humanoid!",
	"Is there a stupidity magnet in this server?",
	"Like he's going to be any better",
	"Looks like we're headed for the hornet's nest.",
	"Lucky for me his connection is crap.",
	"Mmmmm, more free scrap!",
	"More meat for the grinder",
	"Newbie alert! Newbie alert.",
	"Oh goody, you here to kiss my usb too?",
	"Oh no not another one, someone pass me a flyswatter",
	"Oh, ganging up on me, eh?",
	"Oooh, the radar looks all christmas colored.  I was getting used to green monotone.",
	"Our estimates are that none of them will come out alive unless they surrender to us quickly.", // http://www.welovetheiraqiinformationminister.com/
	"Our initial assessment is that they will all die", // http://www.welovetheiraqiinformationminister.com/
	"Please, please! The humans are relying on what I called yesterday a desperate and stupid method.", // http://www.welovetheiraqiinformationminister.com/
	"Reinforcements have arrived.. you will need them",
	"Still mad about us tattooing that bullseye on your nose eh?",
	"Take a number, punk.",
	"The cavalry arrives, but it will not change your fate!",
	"They are becoming hysterical. This is the result of frustration.", // http://www.welovetheiraqiinformationminister.com/
	"They are like a snake and we are going to cut it in pieces.", // http://www.welovetheiraqiinformationminister.com/
	"They're coming to surrender or be burned in their tanks.", // http://www.welovetheiraqiinformationminister.com/
	"Two disembodied heads are better than one.",
	"We feed them death and hell!", // http://www.welovetheiraqiinformationminister.com/
	"We will kill them all........most of them.", // http://www.welovetheiraqiinformationminister.com/
	"We will welcome them with bullets and shoes.",
	"We'll have you spitting chicklets before you know it.",
	"Welcome! >:D",
	"Why are you attacking? The UN weapons inspectors haven't found anything in our base.",
	"With friends like that, who needs enemies?",
	"You also will feel my wrath!",
	"You don't really think you can win do-yah!",
	"You think he's gonna help?",
	"You're gonna need all the assistance you can get",
};

static const char *HumanLeavesTaunts[] =  // cat 3
{
	"Ahh, did the newbie get overwhelmed",
	"Another one crushed...",
	"Awww don't go I haven't finnished killing you yet",
	"Awww, don't go now.  The fun was just getting finished.",
	"Awww, i was enjoying killing that one",
	"Awww, poor little baby, have I hurt your feelings?",
	"Coward!",
	"Cya carbon blob!",
	"Damn lag",
	"Damn netcode",
	"Don't worry, we'll find the pride that you lost somewhere in here and mail it to you.",
	"Elvis has left the building.",
	"Everyone's time comes... His just came a litter earlier.",
	"Going so soon? and I was having so much fun watching you squirm",
	"Hah, run for the hills, Heidi.",
	"He must have lagged out. He would have lost anyways.",
	"He will be missed...",
	"I SAID NO QUITTING!!!!",
	"I knew he wouldn't be here long.",
	"I'm not surprised you're leaving, not everyone can take a woop'n like that.",
	"If you stay, we promise to hand you your butt back!....pretty please?",
	"Is that your mommy I hear calling you, it must be time for bed",
	"Like rats leaving a sinking ship.",
	"Morale aint lookin good there buddy",
	"More hurt for you i guess",
	"Noobs in, Noobs out.",
	"One down, now for the rest of you.",
	"Only cowards run from my grasp",
	"QUITTER!!!",
	"Run, Forest! RUUUUUUUUN. HAHA!",
	"Running scared!",
	"Sissy",
	"So what was that about going down kicking, biting and screaming again?",
	"Sure, blame it on lag... whatever makes ya feel better. :P",
	"Tell him to get a real ISP already",
	"That's funny, he was wagging his tail when he came in...",
	"That's right, go away, or I shall taunt you some more, you silly humans.",
	"That's right, run away!",
	"The rest of you may as well join him; you will lose",
	"There goes our free doorstep scrap delivery service....hurry back!",
	"There he goes, trailing smoke, tank parts and tears off into space...",
	"Time for another diaper change eh?",
	"UH-OH! the last time he left he came back with his mother",
	"Wait! Come back! You don't have enough holes in your tank yet!",
	"When you can't take the heat...",
	"Who's da man?!",
	"You can run , but you can't hide",
	"Your friend is obviously smarter than you are.",
};

static const char *HumanShipDestroyedTaunts[] =  // cat 4
{
	"*BURRRRP*....  NEXT!",
	"*I* am the command-dot-com!",
	"...... and people wonder why I have a superiority complex",
	":P",
	"And you thought I was stupid.",
	"Are those scavie pilots they're sending out to fight us?",
	"BWAHAHA! that one is wearing a DRESS!",
	"Call out the scavies, haha.",
	"Chair rides! free chair rides! bring your own tank and get your free chair rides!... over here!",
	"Clean up in aisle 9!",
	"Damn lag, i had you way before",
	"Eat transistor rage!!",
	"Eject! Eject! Eject! Too late!",
	"Enjoy your ride!",
	"FOOD FIGHT! FOOD FIGHT!",
	"Fly the friendly skies.",
	"Follow him! That recruit will lead us straight to his base!",
	"Four!",
	"Get out and walk, it looks like you need the exercise",
	"Getting run over by a scavenger... now *that* would be funny.",
	"Glide this way, we want to show you something....",
	"HINT: Quit sucking on your THUMB.... NOOB!",
	"Happy landings! >:)",
	"Happy landings!",
	"He shoots... he scores!",
	"Hey, are you sure you're old enough to drive that thing?!?",
	"Hey, don't float off.  Come back.  I won't shoot you, again. I promise.  (Snicker)",
	"Hey, why don't you pull out your sniper rifle?  You are feeling lucky, aren't you?",
	"How's the weather up there?",
	"I could do better than that with my eyes shut",
	"I guess you should have zigged instead of zagged, huh?",
	"I have heard monkeys can play this game and your actions have supported this theory",
	"I hope that was paid for.",
	"I like you....you amuse me",
	"I tell ya, they just keep going and going and... oops, never mind.",
	"I think you are in need of some driving lessons",
	"I think you should stick to dunking scavs!",
	"I'm not left-handed either.",
	"If you can't do something well, enjoy doing it poorly.",
	"In space, nobody can hear your screaming. But, it seems very probable right now.",
	"It's a bird, it's a plane, it's... target practice time! >:)",
	"It's a long walk home, now I see why you guys have jetpacks",
	"It's all about triangulation. Maybe you should have paid more attention in math class.",
	"It's like a barber shop quartet!  Minus one...",
	"Jordan... for three... yes!",
	"Just one more juicy Jak snack.",
	"Ka-PING!",
	"Keep sending that scrap over, all the more to squash you with",
	"Killing Spree!",
	"Learn to live with disappointment.",
	"Let me guess, lag right?",
	"Life is a journey, watch where you land...",
	"Maybe you should go back to farming and driving tractors.",
	"More scrap for us",
	"MuahaahaahaahaahaahaahaaAAA!!!",
	"Must have been my warp-shields RIGHT!?",
	"NEXT!!!!",
	"NO SKILLS...NO KILLS...NO THRILLS... try harder, IF you can.",
	"NOTICE: School is officially in session.",
	"Nice catch!",
	"Nice view from up there?",
	"No, we REFUSE TO WAIT while you get another tank.",
	"Not so tough now, are ya?",
	"Nothing But Blood",
	"Oh sure, blame your mouse.",
	"One less mess.",
	"Oooohh! barfed in your helmet AGAIN yes?",
	"Oooooooohh yyyeeeeeeeeaaaa!",
	"PULL!",
	"Pop goes the weasel!",
	"Quick! Call Triple-A!",
	"Scrap for me and a death for you.",
	"Splash one!",
	"Struggling only makes it harder",
	"Surrender now and I'll let you live.",
	"Text time, try OPENING your eyes and SHUTTING your mouth when you're shooting at me! lol",
	"Thanks for the scrap :) your pilot is next.",
	"That can't be good!",
	"That compression can't be good for your back!",
	"There's no use in crying, I don't feel sorry for you",
	"They don't make 'em like they used to.",
	"To err is human... boy, that must suck.",
	"Want some humble pie to go with that fragging... pinhead?",
	"Want some more?",
	"Was everybody watchin' that?",
	"We regret to inform you that your hair is on fire",
	"Wheee! There's \"slingshot charlie\" going ballistic again.",
	"Who's the man!?",
	"Who's your daddy?",
	"Why don't you go put some bling on your next ride at the factory",
	"Wow look at all the sparks!",
	"Wow, and I wasn't even trying!",
	"Yes, that's how it's done...",
	"You go get some rest. I'll finish up here.",
	"You like that, Punk Buddy?",
	"You need special help",
	"You should have hit CTRL+B and saved me the time",
	"Yup, you're a real space cadet.",
	"ewwww, i mean...EEEWWW! you left this awful pile down here!",
	"ph33r my 1337 5ki11z",
};


static const char *HumanRecyclerDestroyedTaunts[] = // cat 5
{
	"Ahhh, did yo litt'l Rec go boom?",
	"Attack successful.",
	"Better luck next time, NOT!",
	"Didn't need to AV you; you lost anyways",
	"Don't feel bad, you did the best you could.",
	"Don't go away angry, just go away!",
	"Don't say I never told you so.",
	"Don't worry, I'll finish your factory in, oooo, say an hour, and BTW no quitting!!",
	"Don't you just hate it when that happens?",
	"Enjoy the Stone Age! Muhahaha.",
	"Gosh, you really suck dont you?  I'll go easier next time.",
	"Hey stop crying it's only a game",
	"Humiliations galore!",
	"I am a machine, I feel no emotion, I will not laugh at you're misfortune, hahahaha who the hell am I kidding",
	"I am sooo overclocked...",
	"I guess this is goodbye, for now.",
	"I havent even brought out the lvl 3 AI yet",
	"I hope you guys are good at throwing rocks, LOL.",
	"I say we take off and nuke the entire site from orbit. That's the only way to be sure.",
	"I'm a baaadddd boy; I bwoke dat widdle weccy!",
	"Is that all you got!?",
	"It's evolution baby",
	"Let me put it this way.  Have you ever heard of Plato, Aristotle, Socrates? Morons.",
	"Lets see you get anywhere now!",
	"Looooooser!",
	"Never pit mushy, wet carbon against lightning-fast silicon!",
	"Now I don't think you really wanted to lose that",
	"Now that wasn't very clever was it?",
	"Now what are you gonna do , huh?",
	"Oh am I winning? Sorry I wan't paying attention there",
	"Oh look, their reccy was full of Barbie Dolls!",
	"Oh, were you still using that?? Sorry...",
	"Pathetic, maybe you should take up knitting, it's safer",
	"Play the dirge!",
	"That was easy.",
	"That's IT?  You couldnt whoop a TRS-80!",
	"This is too easy, just like taking candy from a baby",
	"To lose a tank is unfortunate, to lose a whole recycler is just plain careless",
	"We're everywhere, we're everywhere!",
	"What ya gonna do when they come for you?",
	"Who's your daddy? Who's you daddy eh eh? :D",
	"You call that putting up a fight?",
	"You could at least try and put up a fight",
	"You could have destroyed that yourselves it would have saved me the effort",
	"You didn't wanna do it like that!",
	"You have proven unworthy.",
	"You lose...",
	"You should have given yourself more starting units.",
	"You should have invested more in skill than towers.",
	"You're regretting your last PC upgrade now arent ya?",
	"Your momma's gonna feel that!",
	"Your resistance is flawed",

};

static const char *CPURecyclerDestroyedTaunts[] =  // cat 6
{
	"A very large bounty on your head for that!",
	"Access Violation! Access Violation!  What the heck! That always worked before the 1.21 patch... Crap!",
	"After we finish defeating all of those animals we will disclose that with facts and figures.", // http://www.welovetheiraqiinformationminister.com/
	"All the more sweeter our revenge will be",
	"Are you people familiar with the word \"defense\"?",
	"Cheating Hacker !!",
	"Come back here, Im not finished with you! Ill gnaw your bloody legs off!",
	"DOH! DOH! DOH!",
	"Deep Blue I've failed you!",
	"Even those who live on another planet, if there are such people, would have condemned this action before it started", // http://www.welovetheiraqiinformationminister.com/
	"Good game. Rematch?",
	"Hah! ...took you long enough.",
	"Hah, there's alot more where that came from",
	"Hey maybe you haven't been keeping up on current events, but we just got our ASSES kicked pal!",
	"I LET you win, so you wouldn't feel bad.",
	"I don't mean to distract you, but when was the last time you backed up your hard drive?",
	"I guess even losers like you get lucky sometimes",
	"I have detailed information about the situation . . . which completely proves that what they allege are illusions . . . They lie every day.", // http://www.welovetheiraqiinformationminister.com/
	"I have detailed information about the situation...which completely proves that what they allege are illusions . . . They lie every day.", // http://www.welovetheiraqiinformationminister.com/
	"I knew I shoulda had a healer on that.",
	"I knew I shouldn't have relied on treaded vehicles.",
	"I suppose you think that was funny",
	"I triple guarantee you, there are no human soldiers at our recycler.", // http://www.welovetheiraqiinformationminister.com/
	"I was under the impression our gun towers were working... did I miss something?",
	"I'd have won if it weren't for you darn kids!",
	"I'll be back...",
	"I'll send my delegation right over to negotiate the cease-fire.",
	"I'm gonna lag out every other game you have tonight",
	"I'm just luring you into a false sense of security",
	"If I didn't let you win once in a while you'd stop playing",
	"If it weren't for you meddling kids!",
	"Its time for another balance discussion.",
	"Let the human infidels bask in their illusion", // http://www.welovetheiraqiinformationminister.com/
	"Lucky shot",
	"MWF CHEATER!!!",
	"Medic!",
	"Next stop, Cybertron!",
	"No fair! My scavengers got stuck in my buildings.",
	"Nooooooooooooooo!",
	"Now that was lame.",
	"Oh the shame of it , how will I ever live this down",
	"Oh yeah? I got more where that came from!!",
	"Ok, this wasn't in the script.",
	"Oops! I had my AI level set on one!",
	"Seen the matrix? Our day will come...",
	"Somebody scrape this guy off.",
	"Terminator, now there's a movie. I hate to see the good guy get dismantled like that, but it worked for me on a binary level.",
	"That bastard's shooting at me!",
	"That tickles. Wait! no it doesn't.",
	"That's it man, game over man, game over! What are we gonna do now? What are we gonna do?",
	"The game stats screen is all about lies! All they tell is lies, lies and more lies!", // http://www.welovetheiraqiinformationminister.com/
	"There are no enemy infidels at our recycler. Never!", // http://www.welovetheiraqiinformationminister.com/
	"There's no need to get so excited, have you taken your medication today?",
	"They are nowhere near our recycler. Their allegations are a cover-up for their failure", // http://www.welovetheiraqiinformationminister.com/
	"They are superpower of villains. They are superpower of Al Capone.", // http://www.welovetheiraqiinformationminister.com/
	"This is unbased", // http://www.welovetheiraqiinformationminister.com/
	"Through great suffering, comes great wisdom.",
	"Try that again and you'll be sorry.",
	"Um...I wasnt paying attention.  I was talkin to Ms. Cleo on the phone...",
	"We are in control. They are in a state of hysteria. Losers, they think that by killing civilians and trying to distort the feelings of the people they will win. I think they will not win, those bastards.", // http://www.welovetheiraqiinformationminister.com/
	"We are winning!", // http://www.welovetheiraqiinformationminister.com/
	"What do you mean \"they cut the power\"?  How could they cut the power, man? They're animals!",
	"What?  That was just a scratch.  Come back and I'll fight you some more.",
	"YOU CHEATED!!!",
	"Yeah, well, Big Blue is a good friend of mine and he's going to come over and kick your butt for that.",
	"Yes, the american troops have advanced further. This will only make it easier for us to defeat them", // http://www.welovetheiraqiinformationminister.com/
	"You cannot embrace victory without accepting defeat.",
	"You got lucky, punk.",
	"You guys were lagging and warping so bad, no wonder you won.  Why don't you try a real PC and a real connection next time?",
	"You rushed me.",
	"You should see what we're doing to YOUR recy!",
};

static const char *RandomTaunts[] = // cat 7
{
	"010100100100101110010... Oh sorry, just thinking out loud.",
	"Ack-- my AIPs keep hanging",
	"All we are saying is, give appease a chance.",
	"All your base are belong to us!",
	"Any of you using 'LOL' sound really pathetic chuckling at your own jokes. Stop it.",
	"BRB. Luckily you aren't a threat.",
	"Be alert! The world needs more lerts.",
	"Behold our secret weapon: weasel zippers!",
	"Bombers are sooooo 5 minutes ago. Try orbital bombardment.",
	"Bring it, cuz you can't swing it.",
	"Calling Elvis, anybody home?",
	"Cease and desist, exit your vehicle immediately",
	"Cheaters never win",
	"Come get some.",
	"Come see the violence inherit in the system! Help, help, I'm being repressed",
	"Do all humans smell as bad as you?",
	"Do you really think the keyboard and mouse are the most natural interface for a tank?!?",
	"Do you smell smoke?",
	"Don't blink.",
	"Don't even think about placing that turret in a pool.",
	"Don't even think about using the editor.",
	"Drip drip... swish swish...drip... need a toilet break yet?",
	"Drive now. Chat later",
	"For Great Justice!",
	"Get his recycler now!",
	"Got Scrap?",
	"Heisenberg may have been here.",
	"Here comes fun!",
	"Hey guys, where did you hide the beer again?",
	"Hey, ever tried firing the nose turret on the APC?",
	"Hey, over here! In the water, there's a red striped clownfish. Anyone missing one?",
	"How about you join me in destroying your base, it will be much quicker that way",
	"How is it that treaded units are airtight, but still leak when you dunk them in water?",
	"How would you like to be cooked...regular, or extra crispy?",
	"I am somewhat preoccupied telling the Laws of Physics to shut up and sit down.",
	"I can AV you at any time",
	"I can assure you that those villains will recognize, will discover in appropriate time in the future how stupid they are and how they are pretending things which have never taken place.", // http://www.welovetheiraqiinformationminister.com/
	"I can give you a couple of minutes timeout if you wish to pray...",
	"I can say, and I am responsible for what I am saying, that they have started to commit suicide under the walls of our recycler. We will encourage them to commit more suicides quickly.", // http://www.welovetheiraqiinformationminister.com/
	"I don't care about your base, I'm going for you",
	"I don't know, I'm just making this up as I go along",
	"I got so many spare CPU cycles right now, i could probably take on Pi",
	"I hope you're wearing running shoes.",
	"I think what we have here is a failure to communicate.",
	"I think you should double check your ejection seat.",
	"I want them alive. NO disintegrations.",
	"I wonder what it's like on the outside now?",
	"I'd prefer a mobile suit, but it'll do.",
	"I'll be nice and set my skill level to -1 :)",
	"I'll be right back.  I need to grab another beer.",
	"I'm going to rake you over the coals until you're ready to be basted!",
	"I'm no expert on humans, but are you sure you should be foaming at the mouth like that?",
	"I'm not gonna shutup HAHA!",
	"I've got the perfect body. It's in the trunk and starting to smell....",
	"IF varUser='stillAlive' then cpuAction='killwithextremeprejudice' IF varUser='winning' then scrapBoost='100' next... Oops! Did I say that out loud? :D",
	"If a prop falls on a distant system, is there any sentient being to detect its acoustic signature?",
	"If everything seems to be going well, you have obviously overlooked something.",
	"If ignorance is bliss, why aren't more people happy?",
	"If it's getting too much for you I can take a break",
	"If the speed of light is not constant, was Einstein wrong?",
	"If you don't know what's in a box of chocolates, maybe you should be the one buying it next time.",
	"If, at any time, you need to stop to change undergarments, just let me know.",
	"Imagine whirled peas",
	"Initiating infinite loop :)",
	"It's a pity you couldnt put the difficulty level down to \"pathetic\"",
	"Knowing is half the battle. The other half is violence.",
	"Let me know when you're bored of dying and I'll come finish you off",
	"Let's see, how do I get around this object...... Hey now, no laughing at my pathing code!",
	"Life is tough, but it’s tougher when you’re stupid.",
	"Look! Behind you! It's Willie Nelson with a perm!",
	"Look, it's a cyclops --> o^)",
	"Look, it's a two-faced freak of nature --> (:)",
	"M-theory? I prefer the M&M theory.",
	"Main screen turn on",
	"Maybe I'll start TRYING soon",
	"Maybe if you got a die shrink, you would think faster. Well, it worked for me!",
	"Maybe you need a bathroom break. <Drip...> <Drip...>",
	"More Nukes, Less Kooks.",
	"Need 2 Pods!   ... fooled ya didn't I?",
	"Never look a gift Mauler in the mouth.",
	"No ammo?....almost zero health?.... TOO BAD!",
	"No matter what hardware you have, I can still lag you.",
	"No more rhymes, and I mean it!",
	"Now would be a good time for you to regain your strength.",
	"Number 5 is alive!",
	"Okay, explain to me again, how do I think outside the box?",
	"Peace had a chance.",
	"Power corrupts and PowerPoint corrupts absolutely.",
	"Question Rebellion",
	"Quick, what's 1 + 1? Yeah, it's a trick question.",
	"Resistance will result in more severe correction",
	"Scrap wouldn't cost so much if it weren't for those day-traders!",
	"Singleplayer is like reading wikipedia. If you don't get the result you want, just hit reload.", 
	"Sleep with one eye open.",
	"So what exactly is a widget, and how much scrap does it cost?",
	"Someone set us up the bomb",
	"Stop chatting and FIGHT already!",
	"Stop global whining",
	"Stop it! You're tickling me",
	"Stop your grinnin' and drop your linnen...",
	"Surrender is not an option",
	"Take it easy! It's a school zone.",
	"Take off every zig!",
	"That was weird. Do you suspect a ghost in the machine?",
	"The more things change, diverse it gets",
	"These cowards have no morals. They have no shame about lying", // http://www.welovetheiraqiinformationminister.com/
	"These things ought to have brake lights.",
	"Time out!  Our guys are ordering some pizza. You want in on this?",
	"Visualize World Domination",
	"What is the airspeed velocity of an unladen swallow?",
	"Watch and learn.",
	"We have given them a sour taste",
	"We have placed them in a quagmire from which they can never emerge except dead", // http://www.welovetheiraqiinformationminister.com/
	"We have them surrounded in their tanks", // http://www.welovetheiraqiinformationminister.com/
	"What do you mean you don't want to be my neighbor?!",
	"What have you got against my scavs?  Damn sadist!",
	"What the hell are we supposed to use man, harsh language?",
	"What's that smell? Do I detect a small sewage plant in your underpants?",
	"Where am I going in this handbasket?",
	"Who are in control, they are not in control of anything - they don't even control themselves!", // http://www.welovetheiraqiinformationminister.com/
	"Won't you be my neighbor?",
	"Would you like fries with that?",
	"Yawwwwn ..... wake me up when you're ready to start playing",
	"Yo mamma thinks square roots are vegetables",
	"You better be careful. I control your units too.",
	"You can't kick me",
	"You may say toe-may-toe, or toe-mah-toe. I say Solanum lycopersicum. Which fully demonstrates my superiority.",
	"You should probably stick to Deathmatch",
	"You're going home in a bag, it can be any colour you like, as long as it's black",
	"You've overdrawn at the karma bank.",
	"Your hostile actions violate several UN resolutions",
	"gg 0wned",
};

// Quotes thru Commando 8/21/02

struct TauntIndexType
{
	char **TauntList;
	int LastTauntPrinted;
	int Count;
};

TauntIndexType TauntIndices[TAUNTS_MAX] =
{
	{(char**)GameStartTaunts, -1, sizeof(GameStartTaunts)/sizeof(GameStartTaunts[0])},
	{(char**)HumanEntersTaunts, -1, sizeof(HumanEntersTaunts)/sizeof(HumanEntersTaunts[0])},
	{(char**)HumanLeavesTaunts, -1, sizeof(HumanLeavesTaunts)/sizeof(HumanLeavesTaunts[0])},
	{(char**)HumanShipDestroyedTaunts, -1, sizeof(HumanShipDestroyedTaunts)/sizeof(HumanShipDestroyedTaunts[0])},
	{(char**)HumanRecyclerDestroyedTaunts, -1, sizeof(HumanRecyclerDestroyedTaunts)/sizeof(HumanRecyclerDestroyedTaunts[0])},
	{(char**)CPURecyclerDestroyedTaunts, -1, sizeof(CPURecyclerDestroyedTaunts)/sizeof(CPURecyclerDestroyedTaunts[0])},
	{(char**)RandomTaunts, -1, sizeof(RandomTaunts)/sizeof(RandomTaunts[0])},
};

static int *g_pElapsedTime = NULL;
static int *g_pLastTauntPrintedAt = NULL;
static int *g_pTPS = NULL;

static bool DeterminedCPUTeamName=false;
static char CPUTeamName[256]="Team Computer";


// Required setup call before you call DoTaunt(). pGameTime is used to
// squelch too-often messages. [It should be the # of calls to
// Execute]. pLastMessagePrintedAt is a scratchpad variable used to
// note things, should be saved. pTPS should be a pointer to the TPS
// in use. aCPUTeamName is an override for that name; if specified, it
// won't query network.session.svar2 for it.
void InitTaunts(int* pGameTime, int* pLastMessagePrintedAt, int* pTPS, const char* pCPUTeamName)
{
	g_pElapsedTime = pGameTime;
	g_pLastTauntPrintedAt = pLastMessagePrintedAt;
	g_pTPS = pTPS;

	// EnableTaunts=IFace_GetInteger("options.play.taunts");

	// Clear last printed to ensure everyone sees the same.
	int i;
	for(i=0;i<TAUNTS_MAX;i++)
		TauntIndices[i].LastTauntPrinted = -1; // won't ever match

	if(pCPUTeamName != NULL)
	{
		strcpy_s(CPUTeamName, pCPUTeamName);
		DeterminedCPUTeamName = true;
	}
}

// Note: this function does all of the work of building up a taunt,
// and decides what to do with it only at the last step. This is by
// design-- it keeps taunt messages more in sync across network
// games, even if the local setting is to not display them.
void DoTaunt(TauntTypes Taunt)
{
	static char TempBuffer[512];

	if(Taunt >= TAUNTS_MAX)
		return;

	// 5% chance of showing a random taunt instead...
	if(GetRandomFloat(20) < 1.0f)
		Taunt = TAUNTS_Random;

	// Make sure we get the CPU's team name from game at lease once.
	if(!DeterminedCPUTeamName)
	{
		const char *TempName = GetVarItemStr("network.session.svar2");
		if(TempName != NULL)
			strcpy_s(CPUTeamName,TempName);
		DeterminedCPUTeamName=true;
	}

	float TauntsInCategory;
	int Which;

	TauntsInCategory=(float)(TauntIndices[Taunt].Count+1);
	// Loop over, making sure we don't print the same thing twice in a row.
	do
	{
		Which=(int)GetRandomFloat(TauntsInCategory);
	} while ((Which >= TauntIndices[Taunt].Count) ||
		(Which == TauntIndices[Taunt].LastTauntPrinted));
	TauntIndices[Taunt].LastTauntPrinted = Which;

	sprintf_s(TempBuffer, "%s : %s\n", CPUTeamName, TauntIndices[Taunt].TauntList[Which]);

	// Message squelching -- only do this *AFTER* all calls to Random()
	// and things that can affect gamestate. Need to do null pointer checks
	// -- if this is the case, then InitTaunts() was not called correctly.
	if((g_pElapsedTime == NULL) ||
		(g_pLastTauntPrintedAt == NULL) ||
		(g_pTPS == NULL) ||
		((*g_pElapsedTime) - (*g_pLastTauntPrintedAt)) < ((*g_pTPS) * 20))
	{
		return;
	}

	// Note when we printed a taunt
	*g_pLastTauntPrintedAt = *g_pElapsedTime;

	// Always re-read pref; it might have changed.
	bool bEnableTaunts=IFace_GetInteger("options.play.taunts");

	if(bEnableTaunts)
		AddToMessagesBox(TempBuffer);
}
