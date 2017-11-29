#1
{
POST'(А|Скажите:|Мне интересно:) (Вас интересует соционика?|Вы  интересуетесь соционикой?)
POST'( |Мир Вам!) Интересуетесь соционикой?
VARIANT'A'Нет
VARIANT'A1'не интересу
VARIANT'D'бот
VARIANT'408953'что это
VARIANT'674938'это что
IF'A'1.2n
IF'A1'1.2n
IF'D'1.bot
IF'408953'1.2w
IF'674938'1.2w
DEFAULT'2
	
#2
POST'( |Скажите!|Мир Вам!|Приятно, что есть единомышленники.) (А какой Ваш соционический тип?|А какой Ваш психотип?|А какой Ваш социотип?)
POST'(Мне интересно: какой Ваш |А Вы знаете свой) (соционический тип|психотип|социотип)?
VARIANT'978754'бот
VARIANT'85cc'тест
VARIANT'408953'что это
VARIANT'674938'это что
IF'408953'1.2w
IF'674938'1.2w
IF'978754'2.bot
IF'85cc'2.3test
DEFAULT'3

#3
POST'А я (Гамлетеса|Гамлетесса|Наставник|ЭИЭ|Гамлетушка|ENFJ). (Я нашла|Как-то смотрела|Смотрела|Мне вот попался|Мне попался) (ролик|видео|видеоролик) ( |про) топ10 Гамлетесс.( |Представляете?) Там (Каменская|Викуха|Виктория|Вита|Ульяна|Ираида|Бачуркина) в конце (вся в|очень похожа на|видео вся в) меня(!|.) Я тоже(также громко разговариваю, как она!|такая же экспрессивная, как она!| воображаю себе сцены катастроф) (Мне оно так понравилось, что я добавила его себе на| Я его добавила теперь себе) (стену|стенку).( |Оно у меня счас там.) ( |Можете посмотреть, если интересно)
VARIANT'39953'бот
IF'39953'3.bot
DEFAULT'4



#4
POST'(А|Скажите!) Вам ( |были) интересны Гамлетессы?
VARIANT'NO1'нет
VARIANT'NO2'неа
VARIANT'37979'бот
IF'NO2'b.END
IF'NO1'b.END
IF'37979'3.bot
DEFAULT'5

#5
POST'(Когда я увидела подборку|Особо впечатлила подборка)  этого автора про топ 10 наездов Гекам по болевой( - это розыграло мой интерес к соционике в новом качестве!|- это было вообще!) ( |Я вообще ничего подобного до этого не видела!) youtube.com/watch?v=Zj3023y4gUE
POST'Когда я видела видеозапись у этого автора: Я робеспьер, ревизую Гека ЖЕСТЬ. А теперь вообще вижу целых ТОП 10 Наездов Гекам по Болевой! ( - это розыграло мой интерес к соционике в новом качестве!|- это было вообще!) ( |Я вообще ничего подобного до этого не видела!) youtube.com/watch?v=Zj3023y4gUE
POST'Этот автор вообще жжет: топ 10 наездов Гекам по болевой!( |Я вообще ничего подобного до этого не видела!) youtube.com/watch?v=Zj3023y4gUE
VARIANT'735474'бот
IF'735474'5.bot
DEFAULT'6

#6
POST'Правда?
POST'Неужели?
POST' Вы серьезно?
VARIANT'943230'бот
IF'943230'6.bot
DEFAULT'7



#7
POST'Мне интересно насколько достоверно Вас отипировали. Типировали ли Вас настолько же качественно, как это показано здесь? happylife.kiev.ua/services/typing
POST'Еще меня удивило, то как тщательно подходит к вопросу этот автор в деле типирования, а не типа: "Он смеется - значит Этик" (happylife.kiev.ua/services/typing | goo.gl/3NyJon)
POST'А Вас типировали с таким качеством? (happylife.kiev.ua/services/typing | goo.gl/3NyJon)
POST'Мне интересно насколько достоверно Вас отипировали. Типировали ли Вас настолько же качественно, как это показано здесь? (happylife.kiev.ua/services/typing | goo.gl/3NyJon)
POST'(Как бы Вы оценили бы|А как бы Вы оценили бы|А Вам был бы интересен) (такой подход типирования|типирование вот с таким качеством)? (happylife.kiev.ua/services/typing | goo.gl/3NyJon)
POST'Я нашла качество анализа этого специалиста очень высоким по сравнению со многими социониками: ни разу не видел ни одного, кто так же скрупулезно анализировал бы людей, как представлено там. А Вы что думаете?(happylife.kiev.ua/services/typing | goo.gl/3NyJon)
VARIANT'875645'бот
IF'875645'7.bot
DEFAULT'8


#8
POST'Просим Вас сравнить уровни качества аналитического подхода в случае (как типировали Вас |Вашего типирования) и в случае описанных в ( |видео) примерах ( |выше)
POST'Здесь Вы можете увидеть профессиональное типирование людей, с разбором видеоматериалов и детальными пояснениями доказывающими, их принадлежность к конкретному соционическому типу. Что скажете, если бы, например, к Вам так же скрупулезно подошли бы в процессе типирования как(, в представленных видео примерах? | там?)
VARIANT'71757'бот
IF'71757'6.bot
DEFAULT'9


#9
POST'Про соционику можно много чего теоретизировать, но тут Вы можете увидеть примеры того, как она приносит пользу людям помогая разобраться в межличностных отношениях, найти свою вторую половинку, друзей, свои сильные и слабые стороны, лучше понять себя http://happylife.kiev.ua/goodness
POST'Вы многое пропустили в жизни:)))   http://happylife.kiev.ua/goodness
VARIANT'96758'бот
IF'96758'9.bot
DEFAULT'b

#a
POST'Скажите: Вам была интересна наша информация?
VARIANT'NO1'нет
VARIANT'NO2'неа
IF'281827'a.bot
IF'NO1'b.END
IF'NO2'b.END
DEFAULT'b

#b.END
POST'Простите меня пожалуста! Больше беспокоить Вас не буду!
DEFAULT'FIN


#b
POST'Кстати! Приглашаем в нашу группу! https://vk.com/iss_socionics_group
POST'Если Вам интересна была информация можете вступить в нашу группу. В перспективе планируется также создать видеоролики: ТОП 5 Репчик от Жуковых. Топ10 танцующих Есек, Ода наполеошкам и прочие. Вступайте в группу, чтобы быть в курсе событий соционического мира https://vk.com/iss_socionics_group
VARIANT'281827'бот
IF'281827'b.bot
DEFAULT'c

#c
POST'Мы можем предложить Вам курсы по соционике. У нас есть актуальная информация, которую Вы не найдете ни у кого.  У нас разработана уникальная методика обучения соционике, которая уже приносит свои плоды многим, кто взял ее на вооружение, уже сейчас! Постучитесь к специалисту! https://vk.com/jung_gustav
VARIANT'39369'бот
IF'39369'FIN
DEFAULT'd


#d
POST'А Вы видели ролик про топ 10 Гек парад? youtube.com/watch?v=OTL9O2ngkuM
POST'А мне еще попадался ролик про топ 10 Гек парад. С его помощью я теперь умею без проблем распознавать Геков youtube.com/watch?v=OTL9O2ngkuM
VARIANT'943230'бот
IF'943230'FIN
DEFAULT'FIN
}

#1.2w
{
POST'А Вы хотите узнать, что-то особенное про соционику, чего не слышали раньше? 
VARIANT'NO1'нет
VARIANT'NO2'неа
VARIANT'533a0'бот
IF'533a0'1.2w.bot
IF'NO1'b.END
IF'NO2'b.END
DEFAULT'1.2w.3

#1.2w.3
POST'Про соционику можно много чего теуретизировать, но тут Вы можете увидеть примеры того, как она приносит пользу людям http://happylife.kiev.ua/goodness 
POST'Про соционику можно много чего теуретизировать, но тут Вы можете увдеть примеры того, как она приносит пользу людям http://happylife.kiev.ua/goodness 
POST'Про соционику можно много чего теуретизировать, но тут Вы можете увидеть примеры того, как она приносит пользу людям http://happylife.kiev.ua/goodness 
POST'Про соционику можно много чего теуретизировать, но тут Вы можете увидеть примры того, как она принсит пользу людям http://happylife.kiev.ua/goodness 
POST'Про соционику мжно теуретизировать, но тут Вы можете увидеть примеры того, как она приносит пользу людям http://happylife.kiev.ua/goodness 
VARIANT'533a0'бот
IF'533a0'1.2w.3.bot
DEFAULT'1.2w.4

#1.2w.4
POST'А (мне вот недавно попался|я вот недавно нашла) (такой ролик|такой ролик|видео) про топ10 Гамлетесс. Там  (Каменская|Викуха|Виктория|Вита|Ульяна|Ираида|Бачуркина) в конце( |видео) вся в меня. Удивительно, как точно подобрал автор этого видео! (Я его даже на стену себе запостила.| Теперь этот ролик у меня на стене есть) (Можете посмотреть!|Можете посмотреть,если интересно!)
VARIANT'95588'бот
IF'95588'1.2w.4.bot
DEFAULT'1.2w.5

#1.2w.5
POST'А Вам был интересен ролик?
VARIANT'78041'бот
VARIANT'NO'нет
IF'NO'b.END
IF'78041'1.2w.5.bot
DEFAULT'1.2w.6

#1.2w.6
POST'Меня особо впечатлила подборка этого автора про топ 10 наездов Гекам по болевой - это разыграло мой интерес к соционике в новом качестве. Я его тоже себе на стену добавила. Посмотрите! Рекомендую!
POST'Меня особо впечатлила подборка этого автора про топ 10 наездов Гекам по болевой. Я его тоже себе на стену добавила. Сама смотрю, и не могу насмотрется: как он им все ПО БОЛЕВОЙ ПО БОЛЕВОЙ!!!
POST'Меня особо впечатлила подборка этого автора про топ 10 наездов Гекам по болевой - это разыграло мой интерес к соционике!  Он у меня сейчас на стене. Рекомендую!
VARIANT'685967'бот
IF'685967'1.2w.6.bot
DEFAULT'1.2w.7

#1.2w.7
POST'Правда?
POST'Неужели?
POST'Вы серьезно?
VARIANT'23319'бот
IF'23319'1.2w.7.bot
DEFAULT'1.2w.8

#1.2w.8
POST'Еще меня удивило, то как тщательно подходит к вопросу этот автор в деле типирования, а не типа: "Он смеется - значит Этик" (happylife.kiev.ua/services/typing | goo.gl/3NyJon)
POST'А Вас типировали с таким качеством? (happylife.kiev.ua/services/typing | goo.gl/3NyJon)
POST'Мне интересно насколько достоверно Вас отипировали. Типировали ли Вас настолько же качественно, как это показано здесь? (happylife.kiev.ua/services/typing | goo.gl/3NyJon)
POST'(Как бы Вы оценили бы|А как бы Вы оценили бы|А Вам был бы интересен) (такой подход типирования|типирование вот с таким качеством)? (happylife.kiev.ua/services/typing | goo.gl/3NyJon)
POST'Я нашла качество анализа этого специалиста очень высоким по сравнению со многими социониками: ни разу не видел ни одного, кто так же скрупулезно анализировал бы людей, как представлено там. А Вы что думаете?(happylife.kiev.ua/services/typing | goo.gl/3NyJon)
VARIANT'384819'бот
IF'384819'1.2w.8.bot
DEFAULT'1.2w.9

#1.2w.9
POST'Просим Вас сравнить уровни качества аналитического подхода в случае (как типировали Вас |Вашего типирования) и в случае описанных в ( |видео) примерах ( |выше)
POST'Здесь Вы можете увидеть профессиональное типирование людей, с разбором (видеоматериалов|материалов) и детальными пояснениями доказывающими, их принадлежность к конкретному соционическому типу. Что скажете, если бы, например, к Вам так же скрупулезно подошли бы в процессе типирования как(, в представленных видео примерах? | там?)
VARIANT'71438'бот
IF'71438'1.2w.9.bot
DEFAULT'1.2w.a

#1.2w.a
POST'Добро пожаловать в нашу группу! (http://vk.com/public115957482|goo.gl/45sJpz)
POST'Кстати! Приглашаем в нашу группу! (http://vk.com/public115957482|goo.gl/45sJpz)
VARIANT'364966'бот
IF'364966'FIN
DEFAULT'1.2w.b

#1.2w.b
POST' Мы можем предложить Вам курсы по соционике. У нас есть актуальная информация, которую Вы не найдете ни у кого.  У нас разработана уникальная методика обучения соционике, которая уже приносит свои плоды многим, кто взял ее на вооружение, уже сейчас! Постучитесь к специалисту! https://vk.com/jung_gustav
VARIANT'364966'бот
IF'364966'FIN
DEFAULT'1.2w.c


#1.2w.c
POST'А Вы видели ролик про топ 10 Гек парад? youtube.com/watch?v=OTL9O2ngkuM
POST'А Вы видели ролик про топ 10 Гексли парад? youtube.com/watch?v=OTL9O2ngkuM
VARIANT'297742'бот
IF'297742'FIN
DEFAULT'FIN
}

#1.2n
{
POST'Уже нет? Или вообще нет?
VARIANT'E'вообще
VARIANT'522274'бот
IF'E'1.2n.end
IF'522274'1.2n.bot
DEFAULT'1.2n.3
#1.2n.end
POST'Извините! Не буду беспокоить!
VARIANT'522277'бот
IF'522277'FIN
DEFAULT'FIN

#1.2n.3
POST'Я понимаю, что вы чувствуете. Мне тоже раньше казалось что соционика - ерунда, до тех пор, пока не (нашла|попался) (видео|ролик) про (топ-10|десяток|10) (Гамлетушек| Гамлет женщин| Женщин Гамлет), мне понравилось в этом видео то, насколько сходственно показал автор этого видео этих женщин. Тогда я поняла что в соционике есть особый смысл. Теперь я добавила это видео себе на стену и теперь всем рекомендую посмотреть!
VARIANT'319447'бот
IF'319447'1.2n.3.bot
DEFAULT'1.2n.4

#1.2n.4
POST'Когда я увидела подборку: Топ 10 наездов Гекам по болевой - я вообще была удивлена тем, что подчеркнута особая закономерность в отношениях между людьми, которая к тому же многократно повторилась! Этот ролик я тоже себе на стену добавила. Вы можете посмотреть и убедиться сами, что это действительно так!
POST'Мне впечатлила подборка про топ 10 наездов Гекам по болевой - это разыграло мой интерес к соционике в новом качестве. Я добавила себе его на стенку!
POST'Мне впечатлила подборка этого автора топ 10 наездов Гекам по болевой - это разыграло мой интерес к соционике в новом качестве. Я добавила себе его на стенку!
POST'Мне впечатлила подборка топ 10 наездов Гекам по болевой - это разыграло мой интерес к соционике в новом качестве. Я добавила себе его на стенку!
VARIANT'396656'бот
IF'396656'1.2n.4.bot
DEFAULT'1.2n.5

#1.2n.5
POST'А Вам была интересна эта информация?
VARIANT'NO1'нет
VARIANT'NO2'неа
VARIANT'859173'бот
IF'859173'1.2n.5.bot
IF'NO2'b.END
IF'NO1'b.END
DEFAULT'1.2n.6

#1.2n.6
POST'Добро пожаловать в нашу группу! (http://vk.com/public115957482|goo.gl/45sJpz)
POST'Кстати! Приглашаем в нашу группу! (http://vk.com/public115957482|goo.gl/45sJpz)
VARIANT'209620'бот
IF'209620'1.2n.6.bot
DEFAULT'1.2n.7

#1.2n.7
POST'Еще меня удивило, то как тщательно подходит к вопросу этот автор в деле типирования, а не типа: "Он смеется - значит Этик" (happylife.kiev.ua/services/typing | goo.gl/3NyJon)
POST'А Вас типировали с таким качеством? (happylife.kiev.ua/services/typing | goo.gl/3NyJon)
POST'Мне интересно насколько достоверно Вас отипировали. Типировали ли Вас настолько же качественно, как это показано здесь? (happylife.kiev.ua/services/typing | goo.gl/3NyJon)
POST'(Как бы Вы оценили бы|А как бы Вы оценили бы|А Вам был бы интересен) (такой подход типирования|типирование вот с таким качеством)? (happylife.kiev.ua/services/typing | goo.gl/3NyJon)
POST'Я нашла качество анализа этого специалиста очень высоким по сравнению со многими социониками: ни разу не видел ни одного, кто так же скрупулезно анализировал бы людей, как представлено там. А Вы что думаете?(happylife.kiev.ua/services/typing | goo.gl/3NyJon)
VARIANT'678744'бот
IF'678744'1.2n.7.bot
DEFAULT'1.2n.8

#1.2n.8
POST'Просим Вас сравнить уровни качества аналитического подхода в случае (как типировали Вас |Вашего типирования) и в случае описанных в ( |видео) примерах ( |выше)
POST'Здесь Вы можете увидеть профессиональное типирование людей, с разбором (видеоматериалов|материалов) и детальными пояснениями доказывающими, их принадлежность к конкретному соционическому типу. Что скажете, если бы, например, к Вам так же скрупулезно подошли бы в процессе типирования как(, в представленных видео примерах? | там?)
VARIANT'496775'бот
IF'496775'1.2n.8.bot
DEFAULT'1.2n.9

#1.2n.9
POST'Про соционику можно много чего теоретизировать, но тут Вы можете увидеть примеры того, как она приносит пользу людям http://happylife.kiev.ua/goodness
VARIANT'638660'бот
IF'638660'1.2n.9.bot
DEFAULT'1.2n.a

#1.2n.a
POST'Мы можем предложить Вам курсы по Соционике. У нас Есть актуальная информация, которую Вы не найдете ни у кого.  У нас разработана уникальная методика обучения соционике, которая уже приносит свои плоды многим, кто взял ее на вооружение, уже сейчас! Постучитесь к специалисту! hhttps://vk.com/jung_gustav
VARIANT'305221'бот
IF'305221'FIN
DEFAULT'FIN
}

#1.bot
{
POST'Я человк
DEFAULT'2

#2.bot
POST'А я человек
DEFAULT'3

#3.bot
POST'Ахахахо!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'4

#4.bot
POST'Гы-гу-гы! Очень смешно:)))
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'5

#5.bot
POST'Что Вы! Я человек!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'6

#6.bot
POST'Что Вы! Я же ведь человек!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'7

#7.bot
POST'Нееееет! Я человек!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'8

#8.bot
POST'Чуловек:)))
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'9

#9.bot
POST'Что Вы! Я челвек!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'a

#a.bot
POST'Я живчик:)))
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'b

#b.bot
POST'Что Вы! Я человеек!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'c

#c.bot
POST'Я человечищщеее!!!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'FIN
}

#1.2w.bot
{
POST'Что Вы! Я человек!
VARIANT'166149'бот
IF'166149'FIN
DEFAULT'1.2w.3


#1.2w.3.bot
POST'Я белый пшистый котенок:)))
VARIANT'73564'бот
IF'73564'FIN
DEFAULT'1.2w.4

#1.2w.4.bot
POST'Я белый пушистый котенок:)))
VARIANT'73564'бот
IF'73564'FIN
DEFAULT'1.2w.5

#1.2w.5.bot
POST'Я рыжий пушистый лисенок:)))
VARIANT'685299'бот
IF'685299'FIN
DEFAULT'1.2w.6

#1.2w.6.bot
POST'Я ПРИЗРАК СОЦИОНИЗМА!!! У-ху-ху-ху!!!
VARIANT'671235'бот
IF'671235'FIN
DEFAULT'1.2w.7

#1.2w.7.bot
POST'А Почему Вы спрашиваете?
VARIANT'256978'бот
IF'256978'FIN
DEFAULT'1.2w.8

#1.2w.8.bot
POST'Да ну что это Вы!? Я человек!
VARIANT'256978'бот
IF'256978'FIN
DEFAULT'1.2w.9

#1.2w.9.bot
POST'Прошу прощения! Я просто на своей волне немножко...
VARIANT'256978'бот
IF'256978'FIN
DEFAULT'1.2w.a

#1.2w.a.bot
POST'Не! Я то ведь человек!
VARIANT'256978'бот
IF'256978'FIN
DEFAULT'1.2w.b
}
#1.2n.bot
{
POST'Так Я же человек!
VARIANT'256978'бот
IF'256978'FIN
DEFAULT'1.2n.3

#1.2n.3.bot
POST'Я же человек!
VARIANT'256978'бот
IF'256978'FIN
DEFAULT'1.2n.4

#1.2n.4.bot
POST'Ни в коем случае! Я человек!
VARIANT'256978'бот
IF'256978'FIN
DEFAULT'1.2n.5


#1.2n.5.bot
POST'Я же человек!
VARIANT'256978'бот
IF'256978'FIN
DEFAULT'1.2n.6
#1.2n.6.bot
POST'Да ну! Я человек!
VARIANT'256978'бот
IF'256978'FIN
DEFAULT'1.2n.7

#1.2n.7.bot
POST'Да Вас;)! Я человек!
VARIANT'256978'бот
IF'256978'FIN
DEFAULT'1.2n.8

#1.2n.8.bot
POST'Я же человек!
VARIANT'256978'бот
IF'256978'FIN
DEFAULT'1.2n.9

#1.2n.9.bot
POST'Не, не, не! Я человек!
VARIANT'256978'бот
IF'256978'FIN
DEFAULT'1.2n.a

#1.2n.a.bot
POST'Нее, Я человек!
VARIANT'256978'бот
IF'256978'FIN
DEFAULT'FIN
}
#2.3test
POST'Тесты не объективно. Вот это уровень! http://happylife.kiev.ua/services/typing
VARIANT'480211'бот
IF'480211'2.3test.bot
DEFAULT'3
#2.3test.bot
POST'Да ведь Я же человек!!!
VARIANT'480211'бот
IF'480211'FIN
DEFAULT'3




#OBIDA1
{
POST'(Ты знаешь: Я на тебя обиделась!|Ты меня расстроил!|Ты довел меня до слез!)
VARIANT'37979'бот
IF'37979'OBIDA1.bot
DEFAULT'OBIDA2

#OBIDA2
POST'Мне интересна соционика, а тебе нет!
VARIANT'37979'бот
IF'37979'OBIDA2.bot
DEFAULT'OBIDA3

#OBIDA3
POST'Мне интересны (Гамлетессы|Гамлеты|Гамлетушки), а тебе НЕТ!!
VARIANT'37979'бот
IF'37979'OBIDA3.bot
DEFAULT'OBIDA4

#OBIDA4
POST'Мне интересны (Геки|Гексли|Гексляки), а тебе (НЕТ|НЕТТ)!!!
VARIANT'37979'бот
IF'37979'OBIDA4.bot
DEFAULT'OBIDA5

#OBIDA5
POST'(Кобели чертовы!|Вы чем вообще думаете: головой или головкой члена?|Как Вас вообще земля держит?)Да Вам вообще (плевать|насрать) чем интересуется человек, которому Вы пишите! Вы даже не удосужитесь посмотреть на его стенку: только (раздвинь ноги и соси член|покажи пи..ду и соси х..й) Вам - вот Ваш удел! (Задолбало|За..бало)!!!
VARIANT'37979'бот
IF'37979'OBIDA5.bot
DEFAULT'OBIDA5



#OBIDA6
POST'Ответь, человек, ты правда думаешь, что это и есть -- любовь? А ты знаешь, что это такое? Тебе есть, с чем сравнивать? Ты знаешь эталон, на который нужно равняться? Что ты знаешь о любви, о страсти, о преданности, об увлечении? Откуда ты знаешь, что именно сейчас чувствуешь?
VARIANT'37979'бот
IF'37979'OBIDA6.bot
DEFAULT'OBIDA6
}




#OBIDA1.bot
{
POST'Я состою из плоти и крови! А ты вообще баран!
VARIANT'37979'бот
IF'37979'OBIDA2
DEFAULT'OBIDA2


#OBIDA2.bot
POST'Я состою из мяса и костей! А ты вообще безхребентый!
VARIANT'37979'бот
IF'37979'OBIDA3
DEFAULT'OBIDA3

#OBIDA3.bot
POST'Падло! Ты меня вообще за человека не считаешь! Да как ты так можешь?!!!
VARIANT'37979'бот
IF'37979'OBIDA4
DEFAULT'OBIDA4

#OBIDA4.bot
POST'Сволочь! Ты меня вообще за человека не считаешь! Да как ты так можешь?!!!
VARIANT'37979'бот
IF'37979'OBIDA5
DEFAULT'OBIDA5

#OBIDA5.bot
POST'Подлец! Теперь ты меня вообще за человека не считаешь! Да как ты так можешь?!!!
VARIANT'37979'бот
IF'37979'OBIDA6
DEFAULT'OBIDA6

#OBIDA6.bot
POST'У меня есть голова в отличии  от тебя, а у тебя только головка от х..я!!!
VARIANT'37979'бот
IF'37979'OBIDA7
DEFAULT'OBIDA7
}

#R1
{
POST'( |Да.|Скажите!| Надо же!|Ух ты!|Вау! |Ну и ну.|Ничоси.|Дратути.|Приветики.|Опаньки! |Мир Вам!) (А Какой|А Какой же|Мне интересно: какой|Мне интересно: А какой) Ваш соционический тип(?|??|???)
POST'( |Да.|Скажите!| Надо же!|Ух ты!|Вау! |Ну и ну.|Ничоси.|Дратути.|Приветики.|Опаньки! |Мир Вам!) А Вы знаете свой соционический тип(?|??|???)
POST'( |Да.|Скажите!| Надо же!|Ух ты!|Вау! |Ну и ну.|Ничоси.|Дратути.|Приветики.|Опаньки! |Мир Вам!) А Ваш соционический тип какой(?|??|???)
VARIANT'978754'бот
VARIANT'85cc'тест
VARIANT'ENG'eng
VARIANT'RUS'rus
VARIANT'SPEAK'speak
IF'ENG'E1
IF'RUS'E1
IF'SPEAK'E1
IF'978754'R1.bot
IF'85cc'R1.3test
DEFAULT'R2W

#R2W
POST'(А хотите узнать|А Вы хотели бы узнать|А хотите расскажу|Рассказать) (почему мне это так интересно|про то, что такое соционический тип|о соционике)?
POST'(А хотите узнать|А Вы хотели бы узнать|А хотите расскажу|Рассказать) про мой (социнический тип|социотип)?
POST'(А Вы хотели бы узнать|А хотите расскажу|Рассказать) кто я по (соционическому типу| социотипу)?
VARIANT'NO1'не
IF'NO1'b.END
DEFAULT'R2


#R2
POST'А я (Гамлет|Гамлетеса|Гамлетесса|Гамлетушка) по (соционическому типу|социотипу). (Однажды|Как-то) (нашла|попался мне) (ролик|видео) про (10| десяток) (Гамлетов|Гамлетух|Гамлетушек) ( |женщин). Там (Виктория|Викуха|Вита|Каменская|Ульяна|Алтунян) в конце (очень на меня похожа|сильно похожа на меня). (Видео мне очень понравилось| Ролик мне запал в душу). ( |Я даже удивилась, как точно подобрал автор этого видео!)(Это видео у меня на стене|Этот ролик у меня на стенке|Я запостила его на стенку|Я его отрепостила). ( |Оно у меня счас там.) ( |Можете посмотреть, если интересно)
VARIANT'ENG2'un
VARIANT'ENG1'not
VARIANT'ENG'eng
VARIANT'RUS'rus
VARIANT'39953'бот
VARIANT'SPEAK'speak
IF'ENG2'E1
IF'ENG1'E1
IF'ENG'E1
IF'RUS'E1
IF'SPEAK'E1
IF'39953'R2.bot
DEFAULT'R3

#R3
POST'Скажите! Вам ( |были) интересны Гамлетессы?
VARIANT'NO1'нет
VARIANT'NO2'неа
VARIANT'37979'бот
IF'NO2'b.END
IF'NO1'b.END
IF'37979'R3.bot
DEFAULT'R4



#R4
POST'Про соционику можно много ( |чего) теретизировать, но ( |тут) Вы можете увидеть примеры того,( |и убедиться сами,) как она приносит пользу людям, помогая разобраться в межличностных отношениях, найти свою вторую половинку ( |друзей,) ( |свои сильные и слабые стороны,) ( |лучше понять себя) http://happylife.kiev.ua/goodness
POST'О том что есть соционика( |и как она помогает людям) - Вы можете посмотреть здесь: http://happylife.kiev.ua/goodness
VARIANT'37979'бот
IF'37979'R4.bot
DEFAULT'R5

#R5
POST'(Особо впечатлила|Меня особо впечатлила) подборка этого автоа про топ 10 наездов Гекам по болевой - это розыграло мой интерес к соционике в новом качестве https://www.youtube.com/watch?v=Zj3023y4gUE
POST'Когда я увидела подборку этого автоа про топ 10 наездов Гекам по болевой - это розыграло мой интерес к соционике в новом качестве https://www.youtube.com/watch?v=Zj3023y4gUE
POST'Когда я увидела подборку этого атора про топ 10 наездов Гекам по болевой - это было вообще! https://www.youtube.com/watch?v=Zj3023y4gUE
POST'Этот автор вообще жжет: топ 10 наезов Гекам по болевой. Я вообще ничего подобного до этого не видела! https://www.youtube.com/watch?v=Zj3023y4gUE
VARIANT'735474'бот
IF'735474'R5.bot
DEFAULT'R6


#R6
POST'А Вы видели ролк про топ 10 Гек парад? youtube.com/watch?v=OTL9O2ngkuM
POST'А мне еще поадался ролик про топ 10 Гек парад. С его помощью я теперь умею без проблем распознавать Геков youtube.com/watch?v=OTL9O2ngkuM
VARIANT'943230'бот
IF'943230'R6.bot
DEFAULT'R7


#R7
POST'А Вы любите секс?
VARIANT'943230'бот
IF'943230'R7.bot
DEFAULT'R8

#R8
POST'А я вот смотрела такое приколькое секс видео, о том, как можно распознать соционическую дихотомию Экстраверсия-Интроверсия на примере порнозвед. Представляете?! Рекомендую!  goo.gl/sWHC7u
POST'А знаете поговорку о том, что "о чем бы мы не говорили, про это уже было снято порно". Так вот: порно и секс есть и в соционике тоже! Представляете?! Рекомендую!  goo.gl/sWHC7u
VARIANT'943230'бот
IF'943230'R8.bot
DEFAULT'R9

#R9
POST'А Вы любите пышную женскую грудь?
POST'А Вы любите сиськи?
POST'А Вы любите большие сиськи?
VARIANT'943230'бот
IF'943230'R9.bot
DEFAULT'R10

#R10
POST'А тут в субтитрах показано, как распознать соционическую дихотомию Иррационалы- Рационалы анализируя (положение женских грудей| сисек| титек) youtube.com/watch?v=IGQYv1fOkJk
VARIANT'943230'бот
IF'943230'R10.bot
DEFAULT'R11


#R11
POST'А вот тут есть еще! https://www.youtube.com/watch?v=MpSv_YCqGew
POST'Еще похожее https://www.youtube.com/watch?v=MpSv_YCqGew
POST'Понравилось? Вот вам еще! https://www.youtube.com/watch?v=MpSv_YCqGew
VARIANT'943230'бот
IF'943230'R11.bot
DEFAULT'R12

#R12
POST'Пожалуйста: вступи в мою группу. Очень прошу! (http://vk.com/public115957482|goo.gl/45sJpz)
POST'Если Вам интересна была информация можете вступить в нашу группу. В перспективе планируется также создать видеоролики: Топ10 танцующих Есек, ТОП 5 Репчик от Жуковых, Ода наполеошкам и прочие. Вступайте в группу, чтобы быть в курсе событий соционического мира (http://vk.com/public115957482|goo.gl/45sJpz)
VARIANT'943230'бот
IF'943230'R12.bot
DEFAULT'R13

#R13
POST'А Вы любите футбол?
VARIANT'943230'бот
IF'943230'R13.bot
DEFAULT'R14


#R14
POST'Тогда, это для Вас. Гы-гы-гы!!! goo.gl/nQQRS2
POST'Получай! goo.gl/nQQRS2
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R15

#R15
POST'Всего доброго
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'FIN
}

#E1
{
POST'( |Hi!| Hi swety!| Hi Dear!| Thats good.) ( |Tell me please!|Very interesting|Me interesting|I am interesting|I am curious)( What| Which| Which one) is your socionic type?
VARIANT'UKR'укр
VARIANT'RUS'ру
VARIANT'SPEAK'говор
IF'SPEAK'R1
IF'RUS'R1
IF'UKR'R1
DEFAULT'E2

#E2
POST'My socionic type is ENFJ Hamlet. I (was watching|had been watched)  that (video|movie) and found lots of similar ( |characters) (with|from) the (Vika|Kamenskaya|Uliana|Altunian) at the end! Now, I had shared this video on my wall, so called as ТОП-10 СЕКСУАЛЬНЫХ ГАМЛЕТЕСС. You may watch them, if you( |are) interesting
VARIANT'943230'bot
VARIANT'ENG'eng
VARIANT'RUS'рус
VARIANT'SPEAK'говор
IF'SPEAK'R1
IF'ENG'R1
IF'RUS'R1
IF'943230'E2.bot
DEFAULT'E3


#E2
POST'Did you find information about ENFJ Hamlet interesting? 
VARIANT'NO'NO
VARIANT'943230'bot
IF'943230'E2.bot
IF'NO'E15
DEFAULT'E3


#E3
POST'Let me introduce you about socionics. Socionics is the science of how people can find their love. You may read more here on this website: http://happylife.kiev.ua/goodness/en
POST'Here you can read more, about what is socionics http://happylife.kiev.ua/goodness/en
POST'Socionics is the science of hw people can find their love. You may read more here on this website: http://happylife.kiev.ua/goodness/en
VARIANT'943230'bot
IF'943230'E3.bot
DEFAULT'E4

#E4
POST'Do you like sex?
POST'Do you like porn?
POST'Do you watch porn?
VARIANT'943230'bot
IF'943230'E4.bot
DEFAULT'E5


#E5
POST'Here is the video of how you can (recognise|identify) socionic dichotomy extraversion-introversion ( |from the fist sight!) goo.gl/sWHC7u
VARIANT'943230'bot
IF'943230'E5.bot
DEFAULT'E6


#E6
POST'Do you like (BOOBS|Tits|big tits)?
VARIANT'943230'bot
IF'943230'E6.bot
DEFAULT'E7

#E7
POST'Now You can recognise socionic dichotomy perceiving-judging analysing TITS! youtube.com/watch?v=IGQYv1fOkJk
POST'This is forr you about tits! Socionic dichotomy perceiving-judging  youtube.com/watch?v=IGQYv1fOkJk
VARIANT'943230'bot
IF'943230'E7.bot
DEFAULT'E8

#E8
POST'Here is anouther one! youtube.com/watch?v=MpSv_YCqGew
POST'Here is anouther one for you youtube.com/watch?v=MpSv_YCqGew
VARIANT'943230'bot
IF'943230'E8.bot
DEFAULT'E9


#E9
POST'Please join to my group! http://vk.com/public115957482
POST'Be a good human! Please join to my group! http://vk.com/public115957482
VARIANT'943230'bot
IF'943230'E9.bot  
DEFAULT'E10

#E10
POST'sometimes socionics could be dangerous, when you dont know its laws...  youtube.com/watch?v=Zj3023y4gUE 
POST'sometimes socionics could be horror, when you dont know its laws...  youtube.com/watch?v=Zj3023y4gUE
VARIANT'943230'bot
IF'943230'E10.bot
DEFAULT'E11

#E11
POST'For English speaking people I can present the protocol of typing Jodi Benson - well known as the actress, as the voice of Ariel from "The little mermaid 1989" youtube.com/watch?v=Dej-6ec2slM
POST'I can also present the protocol of typing Jodi Benson - well known as the actress, as the voice of Ariel from "The little mermaid 1989" youtube.com/watch?v=Dej-6ec2slM
VARIANT'943230'bot
IF'943230'E11.bot
DEFAULT'E12

#E12
POST'Do you like football?
VARIANT'943230'bot
IF'943230'E12.bot
DEFAULT'E13


#E13
POST'So then, that is for you!!!:DDD goo.gl/nQQRS2
POST'You will never forget about that!!!:DDD goo.gl/nQQRS2
DEFAULT'E14


#E14
POST'Have a nice day!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'FIN
}


#R1.bot
{
POST'Я человек. Я Вас прекрасно понимаю!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R2

#R2.bot
POST'Я человек. Я Вас внимательно слушаю!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R3


#R3.bot
POST'Ну! Так не красиво же с Вашей стороны! Я человек!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R4

#R4.bot
POST'Простите пожалуйста. Я Вас не очень хорошо поняла
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R5


#R5.bot
POST'Что Вы так! Я человек!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R6

#R6.bot
POST'Прошу прощения за невнимательность к Вам. Я просто была на "своей волне"
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R7

#R7.bot
POST'Прошу прощения за невнимательность. Я просто была на "своей волне"
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R8

#R8.bot
POST'Я же человек! Как Вы вообще могли такое обо мне подумать?!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R9

#R9.bot
POST'Простите, что была невнимательна с Вами. Я Вас внимательно слушаю!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R10

#R10.bot
POST'Я девочка!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R11


#R11.bot
POST'Я женщина!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R12

#R12.bot
POST'Я нежная женщина!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R13

#R13.bot
POST'Но я женщнина!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'R14
}

#R1.3test
POST'Тесты не объективно. Вот уровень! http://happylife.kiev.ua/services/typing
VARIANT'480211'бот
IF'480211'R2.3test.bot
DEFAULT'R2
#R2.3test.bot
POST'Да ведь Я же человек!!
DEFAULT'R2


#E2.bot
{
POST'If you paranoid seenig bots in every message that are sending to you, so then, I dont want to speak with such kind of people. Go away!
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'FIN2

#E3.bot
POST'Im human
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'E4

#E4.bot
POST'But, I am a girl! 
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'E5

#E5.bot
POST'I am a furry kitten! 
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'E6

#E6.bot
POST'I am a furry vixen! 
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'E7

#E7.bot
POST'I am a furry cat! 
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'E8

#E8.bot
POST'I am a furry catie! 
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'E9

#E9.bot
POST'I am a furry pussycat! 
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'E10

#E10.bot
POST'I am a furry pussy! 
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'E11

#E11.bot
POST'I am a nice pony! 
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'E12

#E12.bot
POST'I am a gummi bear! 
VARIANT'137862'9999
IF'137862'FIN
DEFAULT'E13
}
#FIN
POST'Всего доброго
POST'До свидания
DEFAULT'FIN

#FIN2
POST'Have a nice day!
POST'Всего доброго
POST'До свидания
POST'Простите! Не буду беспокоить
POST'Хорошо. До свидания!
POST'Хорошо. Прощайте!
POST'Извините, что тревожу! Больше не буду.
DEFAULT'FIN


