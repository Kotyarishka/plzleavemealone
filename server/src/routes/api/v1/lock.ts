import { Router, Request, Response, NextFunction } from "express";
import LocksManager from "../../../classes/LocksManager";

const router = Router();

function checkRequestForLockId(req: Request, res: Response, next: NextFunction) {
    const { lockId } = req.params;

    if (!lockId) {
        res.status(400).send("Lock ID is required.");
        return;
    }

    if (!LocksManager.hasLock(lockId)) {
        res.status(404).send("Lock does not exist.");
        return;
    }

    next();
}

router.get("/all", (req, res) => {
    const locks = LocksManager.getAllLocks();

    let locksArray: Array<{ id: string, locked: boolean | undefined }> = [];
    locks.forEach((value, key) => {
        locksArray.push({ id: key, locked: value });
    });

    res.send(locksArray);
});

router.post("/create", (req, res) => {
    const { lockId } = req.body;

    if (!lockId) {
        res.status(400).send("Lock ID is required.");
        return;
    }

    if (LocksManager.hasLock(lockId)) {
        res.status(302).send("Lock already exists.");
        return;
    }

    LocksManager.addLock(lockId);

    res.send("OK");
});

router.get("/:lockId", checkRequestForLockId, (req, res) => {
    const { lockId } = req.params;
    const isLocked = LocksManager.isLocked(lockId);

    const lockedString = isLocked ? "locked" : "unlocked";

    res.send(lockedString);
});

router.post("/:lockId/lock", checkRequestForLockId, (req, res) => {
    const { lockId } = req.params;

    LocksManager.lock(lockId);

    res.send("OK");
});

router.post("/:lockId/unlock", checkRequestForLockId, (req, res) => {
    const { lockId } = req.params;

    LocksManager.unlock(lockId);

    res.send("OK");
});

const lastKnownDecoupleIds: { [lockId: string]: string } = {};

router.post("/:lockId/card", checkRequestForLockId, (req, res) => {
    const { lockId } = req.params;

    // body format: cardId,decouoleId
    const { body } = req;

    if (!body) {
        res.status(400).send("Body is required.");
        return;
    }

    const [cardId, decoupleId] = body.split(",");

    if (!cardId) {
        res.status(400).send("Card ID is required.");
        return;
    }

    if (!decoupleId) {
        res.status(400).send("Decouple ID is required.");
        return;
    }

    if (lastKnownDecoupleIds[lockId] === decoupleId) {
        return; // ignore duplicate decouple IDs
    }

    lastKnownDecoupleIds[lockId] = decoupleId;
    // @TODO: record that lock was unlocked
});


export default router;