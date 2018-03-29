/*
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <linux/types.h>
#include <linux/device.h>

#include "vspa.h"

/*
 * Sysfs
 * TODO - complete sysfs: registers, bds
 */

static const char *eventName[16] = {
    "?00?", "DMA", "CMD", "REPLY", "SPM", "MB64I", "MB32I", "MB64O",
    "MB32O", "ERR", "?10?", "?11?", "?12?", "?13?", "?14?", "?15?"
};

static const char * const state_name[] = {
    "unknown", "powerdown", "unprogrammed_idle", "unprogrammed_busy",
    "loading", "startup_error", "running_idle", "running_busy"
};

static ssize_t ShowStat(struct device *, struct device_attribute *, char *);
static ssize_t ShowText(struct device *, struct device_attribute *, char *);

static ssize_t SetDebug(struct device *dev, struct device_attribute *pDevAttr,
            const char *buf, size_t count)
{
    vspaDevice_t *pVspaDev = dev_get_drvdata(dev);
    int err;
    unsigned int val;

    err = kstrtouint(buf, 0, &val);
    if (err)
        return err;

    pVspaDev->debug = val;
    return count;
}

static ssize_t ShowDebug(struct device *dev,
        struct device_attribute *pDevAttr, char *buf)
{
    vspaDevice_t *pVspaDev = dev_get_drvdata(dev);

    return sprintf(buf, "0x%08X\n", pVspaDev->debug);
}

static ssize_t ShowVersions(struct device *dev,
        struct device_attribute *pDevAttr, char *buf)
{
    vspaDevice_t *pVspaDev = dev_get_drvdata(dev);

    return sprintf(buf, "0x%08X 0x%08X 0x%08X 0x%08X\n",
            pVspaDev->vspaCtrl.versions.vspa_hw_version,
            pVspaDev->vspaCtrl.versions.ippu_hw_version,
            pVspaDev->vspaCtrl.versions.vspa_sw_version,
            pVspaDev->vspaCtrl.versions.ippu_sw_version);
}

static ssize_t ShowEvents(struct device *dev,
        struct device_attribute *pDevAttr, char *buf)
{
    vspaDevice_t *pVspaDev = dev_get_drvdata(dev);
    eventList_t *ptr;
    size_t ctr, len, totalLen;

    EventListUpdate(&pVspaDev->vspaCtrl.vspaList);

/* TODO why lock is needed only for read accesses */
    spin_lock(&pVspaDev->vspaCtrl.vspaList.eventListLock);
    ctr = 0;
    totalLen = 0;
    list_for_each_entry(ptr, &pVspaDev->vspaCtrl.vspaList.eventsQueuedList, list) {
        len = sprintf(buf, "[%2zu] %-5s %02X %02X %08X %08X\n",
            ctr++, eventName[ptr->type & 0xF], ptr->id,
            ptr->err, ptr->data0, ptr->data1);
        totalLen += len;
        buf += len;
        if (totalLen >= (PAGE_SIZE - 50)) {
            len = sprintf(buf, "...\n");
            totalLen += len;
            break;
        }
    }
    spin_unlock(&pVspaDev->vspaCtrl.vspaList.eventListLock);
    return totalLen;
}

static ssize_t ShowSeqIds(struct device *dev,
        struct device_attribute *pDevAttr, char *buf)
{
    vspaDevice_t *pVspaDev = dev_get_drvdata(dev);
    seqId_t *s;
    int i;
    size_t len, totalLen;

    spin_lock(&pVspaDev->vspaCtrl.controlLock);
    totalLen = 0;
    for (i = 0; i < MAX_SEQIDS; i++) {
        if (pVspaDev->vspaCtrl.activeSeqIds & (1<<i)) {
            s = &(pVspaDev->vspaCtrl.seqId[i]);
            len = sprintf(buf, "[%2d] %02X %04X %2d %4d %4d %08X\n",
                    i, s->cmdId, s->flags, s->cmdBdIndex,
                    s->cmdBufferIdx, s->cmdBufferSize,
                    s->payload1);
            totalLen += len;
            buf += len;
            if (totalLen >= (PAGE_SIZE - 80)) {
                len = sprintf(buf, "...\n");
                totalLen += len;
                break;
            }
        }
    }
    spin_unlock(&pVspaDev->vspaCtrl.controlLock);
    return totalLen;
}

static DEVICE_ATTR(debug,  S_IWUSR | S_IRUGO, ShowDebug,    SetDebug);
static DEVICE_ATTR(eld_filename,     S_IRUGO, ShowText,     NULL);
static DEVICE_ATTR(events,           S_IRUGO, ShowEvents,   NULL);
static DEVICE_ATTR(seqids,           S_IRUGO, ShowSeqIds,   NULL);
static DEVICE_ATTR(state,            S_IRUGO, ShowStat,     NULL);
static DEVICE_ATTR(state_name,       S_IRUGO, ShowText,     NULL);
static DEVICE_ATTR(versions,         S_IRUGO, ShowVersions, NULL);

static struct attribute *attributes[] = {
    &dev_attr_debug.attr,
    &dev_attr_eld_filename.attr,
    &dev_attr_events.attr,
    &dev_attr_seqids.attr,
    &dev_attr_state.attr,
    &dev_attr_state_name.attr,
    &dev_attr_versions.attr,
    NULL
};

const struct attribute_group gAttrGroup = 
{
    .attrs = attributes,
};

static ssize_t ShowStat(struct device *dev,
            struct device_attribute *pDevAttr, char *buf)
{
    unsigned int val;
    vspaDevice_t *pVspaDev = dev_get_drvdata(dev);

    if (pDevAttr == &dev_attr_state)
        val = FullState(&pVspaDev->vspaCtrl);
    else
        val = 0;

    return sprintf(buf, "%d\n", val);
}

static ssize_t ShowText(struct device *dev,
            struct device_attribute *pDevAttr, char *buf)
{
    const char *ptr;
    vspaDevice_t *pVspaDev = dev_get_drvdata(dev);

    if (pDevAttr == &dev_attr_eld_filename)
        ptr = pVspaDev->eldFilename;
    else
        if (pDevAttr == &dev_attr_state_name)
            ptr = state_name[FullState(&pVspaDev->vspaCtrl)];
        else
            ptr = "??";

    return sprintf(buf, "%s\n", ptr == NULL ? "NULL" : ptr);
}
