OBJDIR = $(HDB_ENV)/dataloader
DBUDIR = $(HDB_ENV)/dbutils

TARGET = $(HDB_ENV)/lib/dataloader.jar

OBJS = \
	$(OBJDIR)/ObservationThread.class \
	$(OBJDIR)/ProcessUSACEfile.class \
	$(OBJDIR)/ProcessUSBRHMfile.class \
	$(OBJDIR)/ProcessUSGSfile.class \
	$(OBJDIR)/ProcessWAfile.class \
	$(OBJDIR)/ProcessWAMetadata.class \
	$(OBJDIR)/USACEfileReader.class \
	$(OBJDIR)/WAfileReader.class \
	$(OBJDIR)/WAMetadata.class

DBUS = \
	$(DBUDIR)/Baseloader.class \
	$(DBUDIR)/DataObject.class \
	$(DBUDIR)/DBAccess.class \
	$(DBUDIR)/Logger.class \
	$(DBUDIR)/Observation.class \
	$(DBUDIR)/RBASEUtils.class \
	$(DBUDIR)/StringOps.class

$(TARGET): $(OBJS) $(DBUS)
	cd $(HDB_ENV); /etc/alternatives/java_sdk_1.8.0/bin/jar -cvf $(TARGET) $(subst $(HDB_ENV)/,,$(OBJS)) $(subst $(HDB_ENV)/,,$(DBUS))

clean: 
	rm -f $(OBJS)
	rm -f $(DBUS)
	rm -f $(TARGET)

%.class: %.java
	/etc/alternatives/java_sdk_1.8.0/bin/javac $< -cp $(HDB_ENV)