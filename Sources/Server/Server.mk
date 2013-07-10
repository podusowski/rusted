TARGET:=Server
TYPE:=application

LDFLAGS:=-L$(BUILD) -lServer -lCommon -lCake -lpthread -lboost_system -lxml2 -lsoci_core

RESOURCES:=Server.cfg TestDataBase.xml ShipClasses.xml runTestServer TestDataBase.sqlite3
DEPENDENCIES+=libServer.a libCommon.a libCake.a

$(TARGET_BASE)/TestDataBase.sqlite3: $(TARGET_BASE)/DataBaseSchema.sql $(TARGET_BASE)/TestDataBase.sql
	@rm -f $@
	cat $^ | sqlite3 $@

$(TARGET): $(BUILD)/BigDataBase.sqlite3

$(BUILD)/BigDataBase.sqlite3: $(TARGET_BASE)/genBigDataBase.py $(TARGET_BASE)/DataBaseSchema.sql
	$(TARGET_BASE)/genBigDataBase.py -o $@.sql
	@rm -f $@
	cat $(TARGET_BASE)/DataBaseSchema.sql $@.sql | sqlite3 $@

